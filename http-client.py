import pycurl,json

try:
    from urllib.parse import urlencode
except ImportError:
        from urllib import urlencode

        
from StringIO import StringIO


def get_method(curl, url):
    buffer = StringIO() 
    curl.setopt(curl.URL,url)
    curl.setopt(curl.WRITEDATA, buffer)
    curl.perform()
    curl.close()
    body = buffer.getvalue()
    print(body)

def post_method(curl, url, argument):
    curl.setopt(curl.URL,url)
    post_data = argument
    postfields = urlencode(post_data)
    curl.setopt(curl.POSTFIELDS, postfields)
    curl.setopt(curl.VERBOSE, True)
    curl.perform()
    curl.close()

def delete_method(curl, url):
    curl.setopt(curl.URL, url)
    curl.setopt(curl.CUSTOMREQUEST, "DELETE")
    curl.perform()

def put_method(curl, url, field, name):
    curl.setopt(pycurl.URL,url)
    curl.setopt(pycurl.HTTPPOST, [(field, name)])
    curl.setopt(pycurl.CUSTOMREQUEST, "PUT")
    curl.perform()
    curl.close()


def main():
    c = pycurl.Curl()
    put_method(c,'http://localhost:8081/tests/testpostvars.php')
    
    


if __name__ == "__main__":
    main()
