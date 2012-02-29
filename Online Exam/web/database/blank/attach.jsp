<html>
<%@ page contentType="text/html; charset=iso-8859-1" %>
<%@ page language="java" import="javazoom.upload.*,javazoom.sendmail.Keys,java.util.Vector,java.text.*" %>
<%@ page errorPage="errorhandler.jsp" %>
<jsp:useBean id="upListener" scope="session" class="javazoom.sendmail.UploadStatus"/>
<%-- UploadBean Setup : Begin --%>
<jsp:useBean id="upbean" scope="session" class="javazoom.upload.UploadBean">
  <jsp:setProperty name="upbean" property="storemodel" value="<%= UploadBean.MEMORYSTORE %>"/>
  <jsp:setProperty name="upbean" property="parser" value="<%= MultipartFormDataRequest.STRUTSPARSER %>"/>
  <jsp:setProperty name="upbean" property="parsertmpdir" value="D:/temp"/>
  <% upbean.addUploadListener(upListener); %>
</jsp:useBean>
<%-- UploadBean Setup : End --%>
<% String PREURI = request.getContextPath()+request.getServletPath().substring(0,request.getServletPath().lastIndexOf("/")); %>
<% NumberFormat formatter = new DecimalFormat("0.0"); %>
<head>
<script language="JavaScript"><!--
function uploadfile()
{
  if (document.upload.filename.value.length > 0)
  {
     document.upload.action="<%= response.encodeURL(PREURI+Keys.ATTACH+"?nocache="+System.currentTimeMillis()) %>";
     window.open('attachstatus.jsp?nocache=<%= System.currentTimeMillis() %>','UploadStatus','scrollbars=no,status=no,width=280,height=40');
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
</head>
<body bgcolor="#FFFFFF" text="#000000" onLoad="error()">
<table width="100%" border="0" cellspacing="0" cellpadding="0">
  <tr>
    <td bgcolor="#EEEEEE">
      <table width="100%" border="0" cellspacing="1" cellpadding="0">
        <tr bgcolor="#FFFFFF">
          <td align="center"><font color="#FF0000" face="Verdana, Arial, Helvetica, sans-serif" size="-1"><b>jMailComposer
            : Blank template</b></font><font face="Arial, Helvetica, sans-serif" size="-2"><a href="../contact/">
            </a></font></td>
        </tr>
      </table>
    </td>
  </tr>
</table>
<hr noshade size="1">
<form method="post" action="" name="upload" enctype="multipart/form-data">
  <table width="60%" border="0" cellspacing="1" cellpadding="1" align="center">
    <tr>
      <td align="left">Select a file to attach :</td>
      <td align="left">&nbsp;</td>
      <td align="left">Attachments :</td>
    </tr>
    <tr>
      <td align="left" valign="top" rowspan="2">
        <input type="file" name="filename" size="40">
      </td>
      <td align="center" valign="top">
        <input type="button" name="attach" value="Attach" onClick="uploadfile()">
        <br>
      </td>
      <td align="left" rowspan="2">
        <select name="attachments" size="6">
          <option>--- Message attachments ----</option>
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
      </td>
    </tr>
    <tr>
      <td align="center" valign="bottom">
        <input type="button" name="remove" value="Remove" onClick="removefile()">
      </td>
    </tr>
    <tr>
      <td align="left"><a href="<%= response.encodeURL(PREURI+Keys.COMPOSE+"?nocache="+System.currentTimeMillis()) %>">&lt;&lt;
        Back to composer</a></td>
      <td align="left">&nbsp;</td>
      <td align="left">Total size : <%= formatter.format(size/1024.0) %> KB</td>
    </tr>
  </table>
  <br>
  <br>
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
