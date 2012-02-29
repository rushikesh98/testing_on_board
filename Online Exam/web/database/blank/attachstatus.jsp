<%@ page contentType="text/html; charset=iso-8859-1" %>
<%@ page language="java" import="java.io.*,java.util.*,javazoom.sendmail.UploadStatus" %>
<%@ page errorPage="ExceptionHandler.jsp" %>
<jsp:useBean id="upListener" scope="session" class="javazoom.sendmail.UploadStatus"/>
<html>
<% if (!upListener.isFileUploaded()) { %>
<META HTTP-EQUIV=Refresh CONTENT="1; URL=attachstatus.jsp">
<% } else { upListener.reset(); %>
  <script>window.close();</script>
<% } %>
<head>
<title>Upload Status</title>
</head>
<body bgcolor="#FFFFFF" text="#000000">
<br>
<table border="0" cellspacing="1" cellpadding="1" align="center">
  <tr>
    <td><b><font face="Verdana, Arial, Helvetica, sans-serif" size="-1">Upload progress
      : </font></b></td>
    <td nowrap><b><font face="Verdana, Arial, Helvetica, sans-serif" color="#FF0000" size="-1">
      <%= upListener.getUploadedKBLength() %> KB (<%= upListener.getUploadedRatio() %>
      %)</font></b></td>
  </tr>
</table>
</body>
</html>
