<%-- 
    Document   : CompanyDetails
    Created on : Nov 14, 2008, 12:57:30 PM
    Author     : user
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
	font-size: 18px;
	color: #CC3333;
}
.style2 {color: #003399}
-->
        </style>
</head>
    <body>
    <table width="100%" border="1" bordercolor="#ECE9D8" bgcolor="#FFFFFF">
<tr>
            <td height="56" colspan="2" bordercolor="#622D13"><jsp:include page="CandidateHeader.jsp"/></td>
          </tr>
          <tr>
            <td height="512" bordercolor="#622D13"><jsp:include page="Candidatemenu.jsp"/></td>
            <td bordercolor="#622D13" valign="top">
            
            <table width="684" height="89" border="1" align="center">
      <tr>
        <td height="24" colspan="7" bordercolor="#20450A" bgcolor="#E3DFE2" ><div align="center" class="style63 style8 style1"><strong>Company Details </strong></div></td>
      </tr>
      <tr>
        <td width="88" height="27" bordercolor="#20450A" bgcolor="#CAC297"><div align="center" class="style64 style7 style2"><strong>Name</strong></div></td>
        <td width="97" bordercolor="#20450A" bgcolor="#CAC297" ><div align="center" class="style64 style7 style2"><strong>Address</strong></div></td>
        <td width="101" bordercolor="#20450A" bgcolor="#CAC297" ><div align="center" class="style64 style7 style2"><strong>Post</strong></div></td>
        <td width="88" bordercolor="#20450A" bgcolor="#CAC297" ><div align="center" class="style64 style7 style2"><strong>Criteria</strong></div></td>
        <td width="98" bordercolor="#20450A" bgcolor="#CAC297" ><div align="center" class="style64 style7 style2"><strong>Email</strong></div></td>
        <td width="103" bordercolor="#20450A" bgcolor="#CAC297" ><div align="center" class="style64 style7 style2"><strong>ContactNo:</strong></div></td>
        <td width="79" bordercolor="#20450A" bgcolor="#CAC297" ><div align="center" class="style64 style7 style2"><strong>Cutoff % </strong></div></td>
      </tr>
      <% 
    Class.forName("sun.jdbc.odbc.JdbcOdbcDriver");
      Connection con=DriverManager.getConnection("jdbc:odbc:OnlineExam");
      try
              {
      Statement st=con.createStatement();
      ResultSet rs=st.executeQuery("select * from Com");
        while(rs.next())
            {
   %>
      <tr>
        <td height="28" bordercolor="#20450A" bgcolor="#E9DDD6" ><span class="style6">
        <% out.println(rs.getString("CName"));%>
        </span></td>
      <td bordercolor="#20450A" bgcolor="#E9DDD6" ><span class="style6">
          <% out.println(rs.getString("CAddress"));%>
        </span></td>
        <td bordercolor="#20450A" bgcolor="#E9DDD6" ><span class="style6">
          <% out.println(rs.getString("CPost"));%>
        </span></td>
        <td bordercolor="#20450A" bgcolor="#E9DDD6"><span class="style6">
          <% out.println(rs.getString("CCriteria"));%>
        </span></td>
        <td bordercolor="#20450A" bgcolor="#E9DDD6"><span class="style6">
          <% out.println(rs.getString("CEmail"));%>
        </span></td>
        <td bordercolor="#20450A" bgcolor="#E9DDD6"><span class="style6">
          <% out.println(rs.getString("CContact"));%>
        </span></td>
        <td bordercolor="#20450A" bgcolor="#E9DDD6"><span class="style6">
          <% out.println(rs.getString("CCutoff"));%>
        </span></td>
      </tr>
      <%
      }//while
      }//try
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
