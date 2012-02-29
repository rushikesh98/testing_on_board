
<%@page import="java.io.*,java.net.*,java.util.*,java.text.*," %>
<%@page contentType="text/html" pageEncoding="UTF-8"%>
<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=ISO-8859-1">
<title>Insert title here</title>
</head>
<body>
<% 

  // Get username. 
  String semail = request.getParameter("senderemail" ); 
String demail= request.getParameter("receiveremail" ); 
String subject=request.getParameter("subject" ); 
String content=request.getParameter("email" ); 
out.write(semail);
out.write(demail);
out.write(subject);
out.write(content);

%> 

<% if ( semail == null && semail.equals( "" ) && semail == null && semail.equals( "" ))
{
%> 
Please enter an email address again. 
<% 
} 
else
{ %>
 
<%= request.getParameter( "username" ) %> 
You are registered<%= new java.util.Date() %> 

<!--  Also write out some HTML --> 
Thank you.  A confirmation email has been sent to <%= semail %> 
<% } %> 

<%
private Socket smtpSocket = null;
private DataOutputStream os = null;
private DataInputStream is = null;

Date dDate = new Date();
DateFormat dFormat =  DateFormat.getDateInstance(DateFormat.FULL,Locale.US);

try
{ // Open port to server
  smtpSocket = new Socket(m_sHostName, m_iPort);
  os = new DataOutputStream(smtpSocket.getOutputStream());
  is = new DataInputStream(smtpSocket.getInputStream());
            
  if(smtpSocket != null && os != null && is != null)
  {
 
  }
}
catch(Exception e)
{ System.out.println("Host " + m_sHostName + "unknown"); }
%>

<%

try                        
{   os.writeBytes("HELLO\r\n");

        os.writeBytes("MAIL From: "+ semail +"\n");
             
    // Who the email is going to.
    os.writeBytes("RCPT To: <theperson@theircompany.com>\r\n");
    //IF you want to send a CC then you will have to add this
    os.writeBytes("RCPT Cc: <theCC@anycompany.com>\r\n");

              
    // Now we are ready to add the message and the 
    // header of the email to be sent out.                
    os.writeBytes("DATA\r\n");
                  
    os.writeBytes("X-Mailer: Via Java\r\n");
    os.writeBytes("DATE: " + dFormat.format(dDate) + "\r\n");
    os.writeBytes("From: Me <me@mycompany.com>\r\n");
    os.writeBytes("To:  YOU <you@yourcompany.com>\r\n");
               
    //Again if you want to send a CC then add this.
    os.writeBytes("Cc: CCDUDE <CCPerson@theircompany.com>\r\n");
                
    //Here you can now add a BCC to the message as well
    os.writeBytes("RCPT Bcc: BCCDude<BCC@invisiblecompany.com>\r\n");
    }
                
    sMessage = "Your subjectline.";

    os.writeBytes("Subject: Your subjectline here\r\n");
    os.writeBytes(sMessage + "\r\n");
    os.writeBytes("\r\n.\r\n");
    os.writeBytes("QUIT\r\n");
                
    // Now send the email off and check the server reply.  
    // Was an OK is reached you are complete.
    String responseline;
    while((responseline = is.readLine())!=null)
    {  // System.out.println(responseline);
        if(responseline.indexOf("Ok") != -1)
            break;
    }
}
catch(Exception e)
{  System.out.println("Cannot send email as an error occurred.");  }

%>
</BODY> 
</HTML> 

