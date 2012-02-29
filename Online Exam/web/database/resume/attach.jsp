<html>
<%@ page language="java" import="javazoom.upload.*,javazoom.sendmail.Keys,java.util.Vector,java.text.*" %>
<%@ page errorPage="errorhandler.jsp" %>
<%-- UploadBean Setup : Begin --%>
<jsp:useBean id="upbean" scope="session" class="javazoom.upload.UploadBean">
  <jsp:setProperty name="upbean" property="storemodel" value="<%= UploadBean.MEMORYSTORE %>"/>
  <jsp:setProperty name="upbean" property="parser" value="<%= MultipartFormDataRequest.STRUTSPARSER %>"/>
  <jsp:setProperty name="upbean" property="parsertmpdir" value="D:/temp"/>
</jsp:useBean>
<%-- UploadBean Setup : End --%>
<% String PREURI = request.getContextPath()+request.getServletPath().substring(0,request.getServletPath().lastIndexOf("/")); %>
<% NumberFormat formatter = new DecimalFormat("0.0"); %>
<head>
<LINK REL=STYLESHEET TYPE="text/css" HREF="styles.css">
<script language="JavaScript"><!--
function uploadfile()
{
  if (document.upload.filename.value.length > 0)
  {
     document.upload.action="<%= response.encodeURL(PREURI+Keys.ATTACH+"?nocache="+System.currentTimeMillis()) %>";
     document.upload.submit();
  }
  else
  {
    alert("Select a file to attach");
  }
}
function removefile()
{
  document.upload.action="<%= response.encodeURL(PREURI+Keys.UNATTACH+"?nocache="+System.currentTimeMillis()) %>";
  document.upload.submit();
}
function resumedone()
{
  document.upload.action="<%= response.encodeURL(PREURI+Keys.COMPOSE+"?nocache="+System.currentTimeMillis()) %>";
  document.upload.submit();
}
function error()
{
  <% String errormsg = (String) request.getParameter(Keys.UPLOADERRORMSGKEY);
     if ((errormsg) != null) { %>
    alert("<%= errormsg %>");
     <% } %>
}
//-->
</script>

<title>Add/Remove attachments</title>
<meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1">
</head>
<body bgcolor="#FFFFFF" text="#000000" onLoad="error()">
<table width="100%" border="0" cellspacing="0" cellpadding="0">
  <tr>
    <td bgcolor="#EEEEEE">
      <table width="100%" border="0" cellspacing="1" cellpadding="0">
        <tr bgcolor="#FFFFFF">
          <td align="center"><font color="#FF0000" face="Verdana, Arial, Helvetica, sans-serif" size="-1"><b>jMailComposer
            sample :<font color="#CC0000"> <font color="#FF0000">Email your resume</font></font></b></font><font face="Arial, Helvetica, sans-serif" size="-2">&nbsp;</font></td>
        </tr>
      </table>
    </td>
  </tr>
</table>
<hr noshade size="1">
<form method="post" action="" name="upload" enctype="multipart/form-data">
  <table border="0" cellspacing="1" cellpadding="1" align="center">
    <tr>
      <td align="left" class="Field" nowrap>In addition to your <b>resume</b>,
        you could attach one or more files like articles you wrote ..<br>
        <br>
        Select a file to attach (msword, pdf, rtf format allowed) :<br>
      </td>
    </tr>
    <tr>
      <td align="left" valign="top">
        <input type="file" name="filename" size="40" class="Attach">
        <input type="button" name="attach" value="Attach" onClick="uploadfile()" class="BlueButton">
        <br>
        <br>
      </td>
    </tr>
    <tr>
      <td align="left" class="Field">
        <select name="attachments" size="4" class="Attach">
          <option>----- Your files  ------</option>
          <%
             long size = 0;
             Vector v = (Vector) upbean.getMemorystore();
             if (v!= null)
             {
               for (int i=0;i<v.size();i++)
               {
                 UploadFile up = (UploadFile) v.elementAt(i);
                 size = size + up.getFileSize();
                %>
          <option value="<%= i %>"><%= up.getFileName()+" ("+formatter.format(up.getFileSize()/1024.0)+"KB)" %></option>
          <%}
             }
          %>
        </select>
        <input type="button" name="remove" value="Remove" onClick="removefile()" class="WhiteButton">
      </td>
    </tr>
    <tr>
      <td align="left" class="Field"> <br>
        <input type="button" name="resumeok" value="Done" onClick="resumedone()" class="BlueButton">
	  </td>
    </tr>
  </table>
  <br>
  <hr noshade size="1">
  <table width="100%" border="0" cellspacing="0" cellpadding="0">
    <tr>
      <td align="center"><font face="Verdana, Arial, Helvetica, sans-serif" size="-2"><a href="http://www.javazoom.net/jzservlets/jmailcomposer/">jMailComposer</a>
        &copy; Copyright <a href="http://www.javazoom.net">JavaZOOM</a> 1999-2003</font></td>
    </tr>
  </table>
  <br>
  <br>
</form>
</body>
</html>
