<%@ page contentType="text/html; charset=iso-8859-1" %>
<%@ page language="java" import="javazoom.sendmail.Keys,javazoom.sendmail.ComposeBean,java.util.*" %>
<%@ page errorPage="errorhandler.jsp" %>
<%-- ComposeBean Setup : Begin --%>
<jsp:useBean id="composebean" scope="session" class="javazoom.sendmail.ComposeBean"/>
<jsp:setProperty name="composebean" property="smtpserver" value="smtp.yourcompany.com"/>
<jsp:setProperty name="composebean" property="allowattachment" value="true"/>
<jsp:setProperty name="composebean" property="charset" value="iso-8859-1"/>
<%-- ComposeBean Setup : End --%>
<% String PREURI = request.getContextPath()+request.getServletPath().substring(0,request.getServletPath().lastIndexOf("/")); %>
<html>
<head>
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
</head>
<body bgcolor="#FFFFFF" onLoad="checkForm()">
<table width="100%" border="0" cellspacing="0" cellpadding="0">
  <tr>
    <td bgcolor="#EEEEEE">
      <table width="100%" border="0" cellspacing="1" cellpadding="0">
        <tr bgcolor="#FFFFFF">
          <td><font face="Arial, Helvetica, sans-serif" size="-2">&nbsp;</font><font color="#FF0000" face="Verdana, Arial, Helvetica, sans-serif" size="-1"><b>jMailComposer
            : Blank template</b></font></td>
          <td align="right"><font face="Arial, Helvetica, sans-serif" size="-2"><a href="../contact/index.html"> 
            Contact sample&gt;</a></font></td>
        </tr>
      </table>
    </td>
  </tr>
</table>
<hr noshade size="1">
<form method="post" action="<%= response.encodeURL(PREURI+Keys.SENDMAIL+"?nocache="+System.currentTimeMillis()) %>" name="compose">
  <table border="0" cellspacing="1" cellpadding="0" width="660">
    <tr>
      <td colspan="2">
        <table width="100%" border="0" cellspacing="1" cellpadding="0">
          <tr>
            <td width="15%" align="right" nowrap>From Email : </td>
            <td colspan="2">
              <table border="0" cellspacing="0" cellpadding="0">
                <tr>
                  <td>
                    <input type="text" name="<%= ComposeBean.FROMEMAIL %>" value="<jsp:getProperty name="composebean" property="fromemail"/>" size="38">
                  </td>
                  <td nowrap> &nbsp;From Name : </td>
                  <td>
                    <input type="text" name="<%= ComposeBean.FROMNAME %>" value="<jsp:getProperty name="composebean" property="fromname"/>" size="20">
                  </td>
                </tr>
              </table>
            </td>
          </tr>
          <tr>
            <td width="15%" align="right">To : </td>
            <td width="55%">
              <input type="text" name="<%= ComposeBean.TO %>" value="<jsp:getProperty name="composebean" property="to"/>" size="38">
            </td>
            <td width="30%">&nbsp;</td>
          </tr>
          <tr>
            <td width="15%" align="right">Cc : </td>
            <td width="55%">
              <input type="text" name="<%= ComposeBean.CC %>" value="<jsp:getProperty name="composebean" property="cc"/>" size="38">
            </td>
            <td rowspan="3" width="30%">&nbsp;</td>
          </tr>
          <tr>
            <td width="15%" align="right">Bcc : </td>
            <td width="55%">
              <input type="text" name="<%= ComposeBean.BCC %>" value="<jsp:getProperty name="composebean" property="bcc"/>" size="38">
            </td>
          </tr>
          <tr>
            <td width="15%" align="right">Subject : </td>
            <td width="55%">
              <input type="text" name="<%= ComposeBean.SUBJECT %>" value="<jsp:getProperty name="composebean" property="subject"/>" size="35">
            </td>
          </tr>
          <tr>
            <td width="15%" align="right" nowrap><% if (composebean.getAllowattachment()) {%>Attachments : <%}%></td>
            <td width="55%">
             <% if (composebean.getAllowattachment()) {%>
              <input type="button" name="Attachments" value="Add/Remove Attachments" onClick="attachform()">
              <br>
              <jsp:getProperty name="composebean" property="attachments"/>
             <% } %>
            </td>
            <td align="right" valign="bottom">
              <table border="0" cellspacing="0" cellpadding="0">
                <tr>
                  <td>Priority : </td>
                  <td>
                    <select name="<%= ComposeBean.PRIORITY %>">
                      <%
                  int[] priorityid = {ComposeBean.HIGHEST,ComposeBean.HIGH,ComposeBean.NORMAL,ComposeBean.LOW,ComposeBean.LOWEST};
                  String[] priority = {"Highest","High","Normal","Low","Lowest"};
                  String selected="";
                  for (int i=0;i<priorityid.length;i++)
                  {
                    if (priorityid[i] == composebean.getXpriority()) selected=" selected";
                    else selected="";
                    %>
                      <option value="<%= priorityid[i] %>"<%= selected%>><%= priority[i] %></option>
                      <%
                  }
                  %>
                    </select>
                  </td>
                </tr>
              </table>
            </td>
          </tr>
        </table>
      </td>
    </tr>
    <tr>
      <td colspan="2">
        <textarea name="<%= ComposeBean.MESSAGE %>" cols="80" rows="10" wrap="HARD"><jsp:getProperty name="composebean" property="message"/></textarea>
      </td>
    </tr>
    <tr>
      <td width="50%">
        <table border="0" cellspacing="0" cellpadding="0">
          <tr>
            <td>
              <%
	  	String selrr = "";
		if (composebean.getReturnReceipt() == true) selrr = "checked";
	      %>
              <input type="checkbox" name="<%= ComposeBean.RETURNRECEIPT %>" value="true" <%= selrr %>>
            </td>
            <td> Return receipt</td>
          </tr>
        </table>
      </td>
      <td width="50%" align="right">
        <%-- Custom fields list setup : Begin --%>
        <%-- Sample : <input type="hidden" name="<%= ComposeBean.CUSTOM %>" value="customerid:phone"> --%>
        <input type="hidden" name="<%= ComposeBean.CUSTOM %>" value="">
        <%-- Custom fields list setup : End --%>
        <input type="button" name="send" value="Send" onClick="sendmail()">
        <input type="reset" name="cancel" value="Cancel">
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
