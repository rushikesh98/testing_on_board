<html>
<%@ page language="java" import="javazoom.sendmail.Keys" %>
<%@ page errorPage="errorhandler.jsp" %>
<% String PREURI = request.getContextPath()+request.getServletPath().substring(0,request.getServletPath().lastIndexOf("/")); %>
<% String anyerror = (String) request.getAttribute(Keys.ERRORKEY); %>
<!-- Registered to : <%= (String) request.getAttribute(Keys.REGISTRATIONKEY) %> -->
<head>
<LINK REL=STYLESHEET TYPE="text/css" HREF="styles.css">
<title>Thank you</title>
<meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1">
</head>
<body bgcolor="#FFFFFF" text="#000000">
<table width="100%" border="0" cellspacing="0" cellpadding="0">
  <tr>
    <td bgcolor="#EEEEEE">
      <table width="100%" border="0" cellspacing="1" cellpadding="0">
        <tr bgcolor="#FFFFFF">
          <td><font face="Arial, Helvetica, sans-serif" size="-2"><a href="../contact/">&lt;
            Contact sample</a></font></td>
          <td align="center"><font color="#FF0000" face="Verdana, Arial, Helvetica, sans-serif" size="-1"><b>jMailComposer
            sample :<font color="#CC0000"> <font color="#FF0000">Email your resume</font></font></b></font></td>
          <td align="right"><font face="Arial, Helvetica, sans-serif" size="-2"><a href="../blank/">
            Template &gt;</a></font></td>
        </tr>
      </table>
    </td>
  </tr>
</table>
<hr noshade size="1">
<table border="0" cellspacing="1" cellpadding="1">
  <tr>
    <td><img src="thanks.jpg" width="107" height="105"></td>
    <td valign="top" class="Field">
      <% if ((anyerror != null) && (anyerror.equals(Keys.EMAILSENT))) { %>
      <b>Thanks for your interest</b>, <br>
      we will contact you very soon.
      <% } else { %>
      Cannot send resume : <%= request.getAttribute(Keys.ERRORMSGKEY)%>
      <% } %>
    </td>
  </tr>
  <tr>
    <td><a href="<%= response.encodeURL(PREURI+Keys.COMPOSENEW+"?nocache="+System.currentTimeMillis()) %>" class="homelink">&lt;&lt;
      Back</a></td>
    <td>&nbsp; </td>
  </tr>
</table>
<hr noshade size="1">
<table width="100%" border="0" cellspacing="0" cellpadding="0">
  <tr>
    <td align="center"><font face="Verdana, Arial, Helvetica, sans-serif" size="-2"><a href="http://www.javazoom.net/jzservlets/jmailcomposer/">jMailComposer</a>
      &copy; Copyright <a href="http://www.javazoom.net">JavaZOOM</a> 1999-2003</font></td>
  </tr>
</table>
<p>&nbsp;</p>
<p>&nbsp;</p>
</body>
</html>
