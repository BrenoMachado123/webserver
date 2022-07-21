#!/usr/bin/python3
import cgi, cgitb
from datetime import datetime
import os
from http import cookies

cookie_string = os.environ.get('HTTP_COOKIE')
co = cookies.SimpleCookie()
try:
	co.load(cookie_string)
	co_val = co["UserLastVisit"].value
except:
	co_val = cookie_string
print ("Set-Cookie:UserLastVisit = " + datetime.now().strftime("%m/%d/%Y, %H:%M:%S") + ";", end="\r\n")
#print ("Set-Cookie:Path = /42cgi-bin/cookies.py;", end="\r\n")
#print ("Set-Cookie:Expires = Friday, 22-Jul-2022 23:59:59 GMT;", end="\r\n")
#print ("Set-Cookie:Domain = " + os.environ["REMOTE_HOST"] + ";\r\n")

print ("Content-type: text/html\r\n\r\n")
print ("<html>")
print ("<head>")
print ("<title> Cookies </title>")
print ("<style> body > div p {margin: 0; padding: 0;} .cookie-container a:focus { transform: scale(0.9); } .cookie-container a, .cookie-container a img { cursor: pointer; min-width 60vw; max-height: 85vh; margin: auto; border-radius: 1rem;} </style>")
print ("</head>")
print ("<body style=\"margin: 0; padding: 0; background-size: cover; background-repeat: no-repeat; background-position: center; background-image: url('https://scienceworld.scholastic.com/content/dam/classroom-magazines/scienceworld/issues/2019-20/051120/cookies-in-space/SW-051120-CookiesInSpace-Medium1.jpg')\">")
print ("<div style=\"margin: 0; width: 100%; min-height: 100vh; background-color: rgba(255, 255, 255, 0.15);\">")
if cookie_string and co_val:
	print ("<span class=\"cookies-prediction\" style=\"background-color: rgba(0, 0, 0, 0.4); position: absolute; color: silver;\"> The Big Cookie knows you've visited us before... (" + co_val + ")")
	print ("</span>")
print ("<div class=\"cookie-container\" style=\"min-height: 100vh; display: flex; align-content: center; flex-wrap: no-wrap;\">")
print ("<a href=\"/42cgi-bin/cookies.py\"><img src=\"https://thumbs.dreamstime.com/b/cookie-planet-fantasy-space-illustration-cookie-planet-fantasy-space-illustration-food-galaxy-concept-art-102071068.jpg\" alt=\"cookie\"></a>")
print ("</div>")


print ("<span class=\"cookies-box\" style=\"background-color: rgba(222, 222, 222, 0.8); position: absolute; bottom: 0; left: 0; padding: 1rem; text-align: center; font-size: 15px;\">")
print ("Do you accept all cookies? <button id=\"cookie-yes\">YES</button> <button id=\"cookie-no\">NO</button>")
print ("</span>")


#for param in os.environ.keys():
#   print ("<p><b>%20s</b>: %s </p>" % (param, os.environ[param]))


#print ("<h1>" + os.environ["REQUEST_METHOD"] + "</h1>")
print ("</div>")
print ("</body>")
print ("</html>")

