<%-- 
    Document   : ViewJobDetails
    Created on : Nov 18, 2008, 1:25:33 PM
    Author     : Srishti
--%>
<%@page import="java.io.*,java.sql.*;"%>
<%@page contentType="text/html" pageEncoding="UTF-8"%>
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN"
   "http://www.w3.org/TR/html4/loose.dtd">

<html>
    <head>
        <meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
        <title>www.CarrierMithr.com</title>
        <style type="text/css">
<!--
.style1 {
	font-size: 24px;
	color: #003399;
	font-weight: bold;
}
.style6 {color: #006633; font-weight: bold; }
.style7 {color: #663300}
.style9 {color: #663300; font-weight: bold; }
-->
        </style>
</head>
         <body style="margin:0px" bgcolor="#FFFFFF">
        <table width="100%" border="1" bgcolor="#FFFFFF">
          <tr>
            <td height="107" colspan="2"><jsp:include page="CompanyHeader.jsp"/></td>
          </tr>
          <tr>
            <td width="1" height="512"><jsp:include page="Companymenu.html"/></td>
            <td width="961" bordercolor="#663366" bgcolor="#CC9999" valign="top"><table width="826" height="87" border="1" align="left" bordercolor="#996666" bgcolor="#999933">
          <tr>
            <td height="31" colspan="8" bordercolor="#516415" bgcolor="#CAA899"><div align="center"><span class="style1">Job Details </span></div></td>
          </tr>
          <tr>
            <td width="72" height="23" bordercolor="#516415" bgcolor="#CAA899">
                <div align="center" class="style9">Job ID </div></td>
            <td width="131" bordercolor="#516415" bgcolor="#CAA899"><div align="center" class="style9">Post</div></td>
            <td width="112" bordercolor="#516415" bgcolor="#CAA899"><div align="center" class="style9">Criteria</div></td>
            <td width="90" bordercolor="#516415" bgcolor="#CAA899"><div align="center" class="style9">Vacancies</div></td>
            <td width="101" bordercolor="#516415" bgcolor="#CAA899"><div align="center" class="style9">Salary/Annum</div></td>
            <td width="103" bordercolor="#516415" bgcolor="#CAA899"><div align="center" class="style9">LastDate</div></td>
            <td width="86" bordercolor="#516415" bgcolor="#CAA899"><div align="center" class="style9">Edit</div></td>
            <td width="79" bordercolor="#516415" bgcolor="#CAA899"><div align="center" class="style9">Delete</div></td>
          </tr>
           <% 
    Class.forName("sun.jdbc.odbc.JdbcOdbcDriver");
      Connection con=DriverManager.getConnection("jdbc:odbc:OnlineExam");
      try
              {
      Statement st=con.createStatement();
      ResultSet rs=st.executeQuery("select * from JobDetails");
            String  jid="",jpost="",jcriteria="",jvac="",jsal="",jdate="";
        while(rs.next())
            {
			jid=rs.getString("JobId");
                        jpost=rs.getString("Post");
                        jcriteria=rs.getString("Criteria");
                        jvac=rs.getString("NoOfVacancies");
                        jsal=rs.getString("Salary");
                        jdate=rs.getString("LastDate");

      %>
      <tr>
        <td height="23" bordercolor="#516415" bgcolor="#CAA899" ><span class="style6">
          <%=jid%>
        </span></td>
        <td bordercolor="#516415" bgcolor="#CAA899" ><span class="style6">
          <%=jpost%>
        </span></td>
        <td bordercolor="#516415" bgcolor="#CAA899" ><span class="style6">
          <%=jcriteria%>
        </span></td>
        <td bordercolor="#516415" bgcolor="#CAA899"><span class="style6">
          <%=jvac%>
        </span></td>
        <td bordercolor="#516415" bgcolor="#CAA899"><span class="style6">
          <%=jsal%>
        </span></td>
        <td bordercolor="#516415" bgcolor="#CAA899"><span class="style6">
          <%=jdate%>
        </span></td>
        <td bordercolor="#516415" bgcolor="#CAA899"><div align="center"><a href="EditJob.jsp?jid=<%=jid%>"><strong>Edit</strong></a></div></td>
        <td bordercolor="#516415" bgcolor="#CAA899"><div align="center"><a href="DeleteJob.jsp?jid=<%=jid%>"><strong>Delete</strong></a></div></td>
      </tr>
  
      <%
      }
      }
      catch(Exception e)
              {
          out.println(e);
              }
            %>
          
        </table>
            </td>
          </tr>
        </table>
        
    </body>
</html>
