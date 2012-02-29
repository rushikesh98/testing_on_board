<%@ page language="java" import="javazoom.sendmail.Keys,javazoom.sendmail.ComposeBean,java.util.*" %>
<%@ page errorPage="errorhandler.jsp" %>
<%-- ComposeBean Setup : Begin --%>
<jsp:useBean id="composebean" scope="session" class="javazoom.sendmail.ComposeBean"/>
<jsp:setProperty name="composebean" property="smtpserver" value="smtp.yourcompany.com"/>
<jsp:setProperty name="composebean" property="allowattachment" value="false"/>
<jsp:setProperty name="composebean" property="to" value="support@yourcompany.com"/>
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
<title>Contact technical support</title>
<meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1">
</head>
<body bgcolor="#FFFFFF" onLoad="checkForm()">
<table width="100%" border="0" cellspacing="0" cellpadding="0">
  <tr>
    <td bgcolor="#EEEEEE">
      <table width="100%" border="0" cellspacing="1" cellpadding="0">
        <tr bgcolor="#FFFFFF">
          <td><font face="Arial, Helvetica, sans-serif" size="-2"><a href="../blank/index.html">&lt; 
            Template</a></font></td>
          <td align="center"><font color="#FF0000" face="Verdana, Arial, Helvetica, sans-serif" size="-1"><b>jMailComposer
            sample :<font color="#CC0000"> <font color="#FF0000">Email to support
            </font></font><font color="#FF0000" face="Verdana, Arial, Helvetica, sans-serif" size="-1"><b>contact</b></font></b></font></td>
          <td align="right"><font face="Arial, Helvetica, sans-serif" size="-2"><a href="../resume/index.html">Resume 
            sample &gt;</a></font></td>
        </tr>
      </table>
    </td>
  </tr>
</table>
<hr noshade size="1">
<form method="post" action="<%= response.encodeURL(PREURI+Keys.SENDMAIL+"?nocache="+System.currentTimeMillis()) %>" name="compose">
  <table border="0" cellspacing="0" cellpadding="0" align="center" width="650">
    <tr>
      <td><img src="support.gif" width="140" height="50"></td>
      <td align="center" class="Field">Fill in the following form to contact our
        <b>technical support</b>. <br>
        You will get an answer within next 12h.</td>
      <td bgcolor="#EEEEFF" align="center"><b><font face="Verdana, Arial, Helvetica, sans-serif" size="-1">Tools</font></b></td>
    </tr>
    <tr>
      <td bgcolor="#EEEEEE">&nbsp; </td>
      <td align="center" valign="top">
        <table border="0" cellspacing="0" cellpadding="0">
          <tr>
            <td bgcolor="#FFFFFF">
              <table border="0" cellspacing="1" cellpadding="0">
                <tr>
                  <td width="15%" align="right" nowrap class="MandatoryField">Name
                    : </td>
                  <td>
                    <input type="text" name="<%= ComposeBean.FROMNAME %>" value="<jsp:getProperty name="composebean" property="fromname"/>" size="28" class="CustomerField">
                  </td>
                </tr>
                <tr>
                  <td width="15%" align="right" nowrap class="MandatoryField">Email
                    : </td>
                  <td>
                    <input type="text" name="<%= ComposeBean.FROMEMAIL %>" value="<jsp:getProperty name="composebean" property="fromemail"/>" size="28" class="CustomerField">
                  </td>
                </tr>
                <tr>
                  <td width="15%" align="right" nowrap class="Field">Day phone
                    : </td>
                  <td>
                    <input type="text" name="Day phone" value="<%= composebean.getCustomvalue("Day phone") %>" size="28" class="CustomerField">
                  </td>
                </tr>
                <tr>
                  <td width="15%" align="right" class="MandatoryField">Product
                    : </td>
                  <td>
                    <select name="Product" class="EmailField">
                      <option value="No product selected" selected>- Select a
                      product -</option>
                      <option value="ProductID1">Product Name1</option>
                      <option value="ProductID2">Product Name2</option>
                      <option value="ProductID3">Product Name3</option>
                      <option value="ProductID4">Product Name4</option>
                      <option value="ProductID5">Product Name5</option>
                    </select>
                  </td>
                </tr>
                <tr>
                  <td width="15%" align="right" class="MandatoryField">Subject
                    : </td>
                  <td>
                    <input type="text" name="<%= ComposeBean.SUBJECT %>" value="<jsp:getProperty name="composebean" property="subject"/>" size="22" class="EmailField">
                  </td>
                </tr>
                <tr align="center">
                  <td colspan="2" class="Field">Please provide a detailed description
                    of your problem below :</td>
                </tr>
              </table>
            </td>
          </tr>
          <tr>
            <td bgcolor="#FFFFFF">
              <textarea name="<%= ComposeBean.MESSAGE %>" cols="54" rows="8" wrap="HARD" class="MessageBox"><jsp:getProperty name="composebean" property="message"/></textarea>
            </td>
          </tr>
          <tr>
            <td align="center" bgcolor="#EEEEEE">
              <%-- Custom fields list setup : Begin --%>
              <input type="hidden" name="<%= ComposeBean.CUSTOM %>" value="Day phone:Product">
              <%-- Custom fields list setup : End --%>
              <input type="button" name="send" value="Submit" onClick="sendmail()" class="BlueButton">
            </td>
          </tr>
        </table>
      </td>
      <td valign="top" bgcolor="#EEEEEE">
        <table width="100%" border="0" cellspacing="0" cellpadding="0">
          <tr>
            <td nowrap>&nbsp;<a href="http://babelfish.altavista.com/translate.dyn" target="_blank" class="homelink">AltaVista
              translator</a>&nbsp;</td>
          </tr>
          <tr>
            <td>
              <hr noshade size="1">
            </td>
          </tr>
          <tr>
            <td class="homelink" align="center">&nbsp;<a href="#" class="homelink">F.A.Q</a>&nbsp;</td>
          </tr>
          <tr>
            <td align="center" >&nbsp;<a href="#" class="homelink">Last updates</a>&nbsp;</td>
          </tr>
          <tr>
            <td align="center">&nbsp;<a href="#" class="homelink">Online documents</a>&nbsp;</td>
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
