# Import modules for CGI handling 
import cgi, cgitb 

# Create instance of FieldStorage 
form = cgi.FieldStorage() 

# Get data from fields
if form.getvalue('textcontent'):
   text_content = form.getvalue('textcontent')
else:
   text_content = "Not entered"

print ("Content-type:text/html\r\n\r\n")
print ("<html>")
print ("<body>")
print ("<h1 style=\"text-align:center;\">Entered Text Content is: </h1>")
print ("<p style=\"text-align:center;\">%s</p>" % text_content)
print ("</body>")
