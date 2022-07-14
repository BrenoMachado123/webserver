#!/usr/bin/python3

# Import modules for CGI handling 
import cgi, cgitb 

# Create instance of FieldStorage 
form = cgi.FieldStorage() 

# print ("Content-Length: 2000")
print ("Content-type:text/html\r\n\r\n")
print ("<html>")
print ("<head>")
print ("<title>Query Results</title>")
print ("</head>")
print ("<body>")
# Get data from fields
if form.getvalue('query'):
	query = form.getvalue('query') 
	print("<h3>Results for: {} </h3>".format(query))
	if (query.lower().find("libft") != -1):
		print("<p>Web Scrapper [1 result, from 1 page(s)] </p><hr>\n<a href=\"https://github.com/pulgamecanica/42Course/tree/main/42Core/Libft\">github.com: pulgamecanica/Libft </a>")
	elif (query.lower().find("php") != -1):
		print("<p>webserv [1 result, from 1 route(s)] </p><hr>\n<a href=\"http://localhost:4242/42cgi-bin/info.php\">webserv: 42cgi-bin/info.php </a>")
	elif (query.lower().find("perl") != -1):
		print("<p>webserv [1 result, from 1 route(s)] </p><hr>\n<a href=\"http://localhost:4242/42cgi-bin/test.pl?some=value&id=99\">webserv: 42cgi-bin/test.pl </a>")
else:
   print("No query available...")
print ("</body>")
print ("</html>")
