<html>
<%@ page contentType="text/html; charset=iso-8859-1" %>
<%@ page language="java" import="javazoom.sendmail.Keys" %>
<%@ page errorPage="errorhandler.jsp" %>
<% String PREURI = request.getContextPath()+request.getServletPath().substring(0,request.getServletPath().lastIndexOf("/")); %>
<% String anyerror = (String) request.getAttribute(Keys.ERRORKEY); %>
<!-- Registered to : <%= (String) request.getAttribute(Keys.REGISTRATIONKEY) %> -->
<head>
<title>Task completed</title>
</head>
<body bgcolor="#FFFFFF" text="#000000">
<table width="100%" border="0" cellspacing="0" cellpadding="0">
  <tr>
    <td bgcolor="#EEEEEE">
      <table width="100%" border="0" cellspacing="1" cellpadding="0">
        <tr bgcolor="#FFFFFF">
          <td><font face="Arial, Helvetica, sans-serif" size="-2">&nbsp;</font><font color="#FF0000" face="Verdana, Arial, Helvetica, sans-serif" size="-1"><b>jMailComposer
            : Blank template</b></font></td>
          <td align="right"><font face="Arial, Helvetica, sans-serif" size="-2"><a href="../contact/">
            Contact sample&gt;</a></font></td>
        </tr>
      </table>
    </td>
  </tr>
</table>
<hr noshade size="1">
<p><br>
  <% if ((anyerror != null) && (anyerror.equals(Keys.EMAILSENT))) { %>
  Email successfully sent.
  <% } else { %>
  Cannot send email : <%= request.getAttribute(Keys.ERRORMSGKEY)%>
  <% } %>
</p>
<p><a href="<%= response.encodeURL(PREURI+Keys.COMPOSENEW+"?nocache="+System.currentTimeMillis()) %>">&lt;&lt;
  Back to composer</a></p><br>
<hr noshade size="1">
<table width="100%" border="0" cellspacing="0" cellpadding="0">
  <tr>
    <td align="center"><font face="Verdana, Arial, Helvetica, sans-serif" size="-2"><a href="http://www.javazoom.net/jzservlets/jmailcomposer/">jMailComposer</a>
      &copy; Copyright <a href="http://www.javazoom.net">JavaZOOM</a> 1999-2003</font></td>
  </tr>
</table>
</body>
</html>
