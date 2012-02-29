<%@ page language="java" import="javazoom.sendmail.Keys,javazoom.sendmail.ComposeBean,java.util.*" %>
<%@ page errorPage="errorhandler.jsp" %>
<%-- ComposeBean Setup : Begin --%>
<jsp:useBean id="composebean" scope="session" class="javazoom.sendmail.ComposeBean"/>
<jsp:setProperty name="composebean" property="smtpserver" value="smtp.yourcompany.com"/>
<jsp:setProperty name="composebean" property="allowattachment" value="true"/>
<jsp:setProperty name="composebean" property="to" value="resume@yourcompany.com"/>
<jsp:setProperty name="composebean" property="subject" value="New Resume"/>
<%-- ComposeBean Setup : End --%>
<% String PREURI = request.getContextPath()+request.getServletPath().substring(0,request.getServletPath().lastIndexOf("/")); %>
<html>
<head>
<LINK REL=STYLESHEET TYPE="text/css" HREF="styles.css">
<script language="JavaScript"><!--
function sendmail()
{
  document.compose.submit();
}
<% if (composebean.getAllowattachment()) {%>
function attachform()
{
  document.compose.action="<%= response.encodeURL(PREURI+Keys.ATTACHFORM+"?nocache="+System.currentTimeMillis()) %>";
  document.compose.submit();
}
<% } %>
function checkForm()
{
  var field = "<%= request.getAttribute(Keys.ERRORKEY)%>";
  if (field != "<%= ComposeBean.NOERROR %>")
  {
    alert("<%= request.getAttribute(Keys.ERRORMSGKEY)%>");
    document.compose.<%= request.getAttribute(Keys.ERRORKEY)%>.focus();
  }
}
//-->
</script>
<title>Compose a message</title>
<meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1">
</head>
<body bgcolor="#FFFFFF" onLoad="checkForm()">
<table width="100%" border="0" cellspacing="0" cellpadding="0">
  <tr>
    <td bgcolor="#EEEEEE">
      <table width="100%" border="0" cellspacing="1" cellpadding="0">
        <tr bgcolor="#FFFFFF">
          <td><font face="Arial, Helvetica, sans-serif" size="-2"><a href="../contact/index.html">&lt; 
            Contact sample</a></font></td>
          <td align="center"><font color="#FF0000" face="Verdana, Arial, Helvetica, sans-serif" size="-1"><b>jMailComposer
            sample :<font color="#CC0000"> <font color="#FF0000">Email your resume</font></font></b></font></td>
          <td align="right"><font face="Arial, Helvetica, sans-serif" size="-2"><a href="../blank/index.html"> 
            Template &gt;</a></font></td>
        </tr>
      </table>
    </td>
  </tr>
</table>
<hr noshade size="1">
<form method="post" action="<%= response.encodeURL(PREURI+Keys.SENDMAIL+"?nocache="+System.currentTimeMillis()) %>" name="compose">
  <table width="100%" border="0" cellspacing="0" cellpadding="0">
    <tr>
      <td>
        <table border="0" cellspacing="0" cellpadding="0" align="center">
          <tr>
            <td bgcolor="#FFFFFF">
              <table width="100%" border="0" cellspacing="1" cellpadding="0">
                <tr>
                  <td width="15%" align="right" nowrap class="MandatoryField">Your
                    name : </td>
                  <td>
                    <input type="text" name="<%= ComposeBean.FROMNAME %>" value="<jsp:getProperty name="composebean" property="fromname"/>" size="38" class="EmailField">
                  </td>
                  <td rowspan="3" align="left"><img src="resume3.jpg" width="93" height="83"></td>
                </tr>
                <tr>
                  <td width="15%" align="right" class="MandatoryField">Your email
                    : </td>
                  <td>
                    <input type="text" name="<%= ComposeBean.FROMEMAIL %>" value="<jsp:getProperty name="composebean" property="fromemail"/>" size="38" class="EmailField">
                  </td>
                </tr>
                <tr>
                  <td width="15%" align="right" nowrap class="MandatoryField">
                    <% if (composebean.getAllowattachment()) {%>
                    Your resume :
                    <%}%>
                  </td>
                  <td class="Field">
                    <% if (composebean.getAllowattachment()) {%>
                    <input type="button" name="Attachments" value="Add/Remove file" onClick="attachform()" class="BlueButton">
                    <br>
                    <jsp:getProperty name="composebean" property="attachments"/>
                    <% } %>
                  </td>
                </tr>
                <tr>
                  <td colspan="3" class="Field" nowrap>In addition to your resume,
                    please let us know more about your interest in our company
                    :</td>
                </tr>
              </table>
            </td>
          </tr>
          <tr>
            <td bgcolor="#FFFFFF">
              <textarea name="<%= ComposeBean.MESSAGE %>" cols="80" rows="8" wrap="HARD" class="MessageBox"><jsp:getProperty name="composebean" property="message"/></textarea>
            </td>
          </tr>
          <tr>
            <td align="left" bgcolor="#FFFFFF">
              <%-- Custom fields list setup : Begin --%>
              <input type="hidden" name="<%= ComposeBean.CUSTOM %>" value="">
              <%-- Custom fields list setup : End --%>
              <input type="hidden" name="<%= ComposeBean.PRIORITY %>" value="<%= ComposeBean.HIGH %>">
              <input type="button" name="send" value="Send" onClick="sendmail()" class="BlueButton">
              <input type="reset" name="cancel" value="Reset" class="WhiteButton">
            </td>
          </tr>
        </table>
      </td>
    </tr>
  </table>
  </form>
<hr noshade size="1">
<table width="100%" border="0" cellspacing="0" cellpadding="0">
  <tr>
    <td align="center"><font face="Verdana, Arial, Helvetica, sans-serif" size="-2"><a href="http://www.javazoom.net/jzservlets/jmailcomposer/">jMailComposer</a>
      &copy; Copyright <a href="http://www.javazoom.net">JavaZOOM</a> 1999-2003</font></td>
  </tr>
</table>
</body>
</html>
