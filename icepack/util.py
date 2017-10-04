
import os
import sys
import getpass
import urllib.request
import http.cookiejar

def _progress_bar_hook(block_num, block_size, total_size):
    read_so_far = block_num * block_size
    if total_size > 0:
        percent = read_so_far * 100 / total_size
        s = "\r%5.1f%% %*d / %d" % (
        percent, len(str(total_size)), read_so_far, total_size)
        sys.stderr.write(s)
        if read_so_far >= total_size:
            sys.stderr.write("\n")

    else:
        sys.stderr.write("read %d\n" % (read_so_far,))


def fetch(url, filename, show_progress = True):
    hook = _progress_bar_hook if show_progress else None
    if not os.path.exists(filename):
        urllib.request.urlretrieve(url, filename, hook)


def fetch_nsidc(urls, files, username = None, password = None):
    if not username:
        username = input('NASA EarthData username: ')
    if not password:
        password = getpass.getpass('NASA EarthData password: ')

    for url, file in zip(urls, files):
        print(url)
        _fetch_nsidc(url, file, username, password)


def _fetch_nsidc(url, file, username, password, debuglevel = 0):
    password_manager = urllib.request.HTTPPasswordMgrWithDefaultRealm()
    password_manager.add_password(None, "https://urs.earthdata.nasa.gov",
                                  username, password)

    cookie_jar = http.cookiejar.CookieJar()

    opener = urllib.request.build_opener(
        urllib.request.HTTPBasicAuthHandler(password_manager),
        urllib.request.HTTPHandler(debuglevel=debuglevel),
        urllib.request.HTTPSHandler(debuglevel=debuglevel),
        urllib.request.HTTPCookieProcessor(cookie_jar))
    urllib.request.install_opener(opener)

    request = urllib.request.Request(url)
    #request.add_header('cookie', cookie_jar) <-- this always gives an error!
    response = urllib.request.urlopen(request)

    redirect_url = response.geturl() + '&app_type=401'
    request = urllib.request.Request(redirect_url)
    response = urllib.request.urlopen(request)

    length_txt = response.getheader('content-length')
    if length_txt:
        length = int(length_txt)
        blocksize = max(4096, length // 100)

        size = 0
        while True:
            buffer = response.read(blocksize)
            if not buffer:
                break
            file.write(buffer)
            size += len(buffer)
            print("{}%\r".format(int(size/length * 100)).rjust(5), end='')

        print()
    else:
        file.write(response.read())

