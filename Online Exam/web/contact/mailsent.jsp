<jsp:useBean id="composebean" scope="session" class="javazoom.sendmail.ComposeBean"/>
<html>
<%@ page language="java" import="javazoom.sendmail.Keys" %>
<%@ page errorPage="errorhandler.jsp" %>
<% String PREURI = request.getContextPath()+request.getServletPath().substring(0,request.getServletPath().lastIndexOf("/")); %>
<% String anyerror = (String) request.getAttribute(Keys.ERRORKEY); %>
<!-- Registered to : <%= (String) request.getAttribute(Keys.REGISTRATIONKEY) %> -->
<head>
<LINK REL=STYLESHEET TYPE="text/css" HREF="styles.css">
<title>Email support</title>
<meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1">
</head>
<body bgcolor="#FFFFFF" text="#000000">
<table width="100%" border="0" cellspacing="0" cellpadding="0">
  <tr>
    <td bgcolor="#EEEEEE">
      <table width="100%" border="0" cellspacing="1" cellpadding="0">
        <tr bgcolor="#FFFFFF">
          <td><font face="Arial, Helvetica, sans-serif" size="-2"><a href="../blank/">&lt;
            Template</a></font></td>
          <td align="center"><font color="#FF0000" face="Verdana, Arial, Helvetica, sans-serif" size="-1"><b>jMailComposer
            sample :<font color="#CC0000"> <font color="#FF0000">Email to support
            </font></font><font color="#FF0000" face="Verdana, Arial, Helvetica, sans-serif" size="-1"><b>contact</b></font></b></font></td>
          <td align="right"><font face="Arial, Helvetica, sans-serif" size="-2"><a href="../resume/">Resume
            sample&gt;</a></font></td>
        </tr>
      </table>
    </td>
  </tr>
</table>
<hr noshade size="1">
<br>
<br>
<table border="0" cellspacing="1" cellpadding="1" align="center">
  <tr>
    <td><img src="support.gif" width="140" height="50"></td>
    <td class="Field" align="center">
      <% if ((anyerror != null) && (anyerror.equals(Keys.EMAILSENT))) { %>
      The following email has been <b>successfully</b> sent to our &nbsp;<br>
      customer support team.<br>
      You will get an answer within next 12h.
      <% } else {%>
      <b>Error</b> cannot send email : <%= request.getAttribute(Keys.ERRORMSGKEY) %>
      <% } %>
    </td>
    <td bgcolor="#EEEEFF">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</td>
  </tr>
  <tr>
    <td bgcolor="#EEEEEE">&nbsp;</td>
    <td class="Field">
      <p>
        <% if ((anyerror != null) && (anyerror.equals(Keys.EMAILSENT))) { %>
        <%= composebean.getMessage() %>
        <% } %>
      </p>
      <p>&nbsp;
        <% if ((anyerror != null) && (anyerror.equals(Keys.EMAILSENT))) { %>
        <a href="<%= response.encodeURL(PREURI+Keys.COMPOSENEW+"?nocache="+System.currentTimeMillis()) %>" class="homelink">&lt;&lt;Back</a>&nbsp;
        <% } else { %>
        <a href="<%= response.encodeURL(PREURI+Keys.COMPOSE+"?nocache="+System.currentTimeMillis()) %>" class="homelink">&lt;&lt;Back</a>&nbsp;
        <% } %>
      </p>
    </td>
    <td bgcolor="#EEEEEE">&nbsp;&nbsp;</td>
  </tr>
</table>
<br>
<br>
<hr noshade size="1">
<table width="100%" border="0" cellspacing="0" cellpadding="0">
  <tr>
    <td align="center"><font face="Verdana, Arial, Helvetica, sans-serif" size="-2"><a href="http://www.javazoom.net/jzservlets/jmailcomposer/">jMailComposer</a>
      &copy; Copyright <a href="http://www.javazoom.net">JavaZOOM</a> 1999-2003</font></td>
  </tr>
</table>
<p>&nbsp;</p>
</body>
</html>
