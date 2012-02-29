<%-- 
    Document   : AdminHomePage
    Created on : Dec 3, 2008, 11:39:04 AM
    Author     : Srishti
--%>
<%@page import="java.io.*,java.sql.*"%>
<%@page contentType="text/html" pageEncoding="UTF-8"%>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1" />
<title>www.CarrierMithr.com</title>

       <style type="text/css">
<!--
.body {
        margin:0%;
}
a:link {
	color: #0066CC;
	text-decoration: underline;
}
a:visited {
	text-decoration: underline;
	color: #663399;
}
a:hover {
	text-decoration: none;
	color: #996666;
}
a:active {
	text-decoration: underline;
	color: #FF9999;
}
.style5 {
	color: #006633;
	font-weight: bold;
	font-size: 18px;
}
.style7 {color: #CC0000}
.style8 {color: #000099}
.style11 {color: #CC0000; font-weight: bold; }
.style12 {
	color: #663399;
	font-weight: bold;
}
-->
       </style>
</head>

<body style="margin:0px">
  <table width="100%" height="472" border="1" align="left">
    <tr>
       <td height="45" colspan="2" bgcolor="#666734"><jsp:include page="AdminHeader.html"/></td>
    </tr>
    <tr>
       <td width="1%" bgcolor="#666734"><jsp:include page="Adminmenu.html"/></td>
       <td bgcolor="#FFFFFF" valign="top"><table width="830" height="109" border="1" align="left">
          <tr>
            <td height="31" colspan="16" bordercolor="#663300" bgcolor="#99BBB7" ><div align="center" class="style8 style63 style5">Candidate Details </div></td>
          </tr>
          <tr>
            <td width="89" height="33" bordercolor="#663300" bgcolor="#99BBB7" ><div align="center"><span class="style11">Username</span></div></td>
            <td width="112" bordercolor="#663300" bgcolor="#99BBB7"><div align="center" class="style64 style7"><strong> Name</strong></div></td>
            <td width="104" bordercolor="#663300" bgcolor="#99BBB7" ><div align="center" class="style64 style7"><strong>Address</strong></div></td>
            <td width="106" bordercolor="#663300" bgcolor="#99BBB7" ><div align="center" class="style64 style7"><strong>Phone</strong></div></td>
            <td width="124" bordercolor="#663300" bgcolor="#99BBB7" ><div align="center" class="style64 style7"><strong>Email_id</strong></div></td>
            <td width="109" bordercolor="#663300" bgcolor="#99BBB7" ><div align="center" class="style64 style7"><strong>Category</strong></div></td>
            <td width="86" bordercolor="#663300" bgcolor="#99BBB7" ><div align="center" class="style64 style7"><strong>Delete</strong></div></td>
            <td width="71" bordercolor="#663300" bgcolor="#99BBB7" ><div align="center" class="style64 style7"><strong>Resume</strong></div></td>
        </tr>
        <%
    Class.forName("sun.jdbc.odbc.JdbcOdbcDriver");
      Connection con=DriverManager.getConnection("jdbc:odbc:OnlineExam");
      try
              {
      Statement st=con.createStatement();
      ResultSet rs=st.executeQuery("select Username,Name,Address,Telephone,Email_id,Category from Details");
	  String  uid="",name="",address="",phone="",email="",cat="";
        while(rs.next())
            {
			uid=rs.getString("Username");
			name=rs.getString("Name");
			address=rs.getString("Address");
			phone=rs.getString("Telephone");
			email=rs.getString("Email_id");
			cat=rs.getString("Category");




      %>
        <tr>
          <td height="35" bordercolor="#663300" bgcolor="#99BBB7" ><div align="center"><%=uid%></div></td>
          <td bordercolor="#663300" bgcolor="#99BBB7" ><div align="center"><%=name%></div></td>
          <td bordercolor="#663300" bgcolor="#99BBB7" ><div align="center"><%=address%></div></td>
          <td bordercolor="#663300" bgcolor="#99BBB7" ><div align="center"><%=phone%></div></td>
          <td bordercolor="#663300" bgcolor="#99BBB7" ><div align="center"><%=email%></div></td>
          <td bordercolor="#663300" bgcolor="#99BBB7"><div align="center"><%=cat%></div></td>
          <td bordercolor="#663300" bgcolor="#99BBB7"><div align="center"><a href="Delete.jsp?uid=<%=uid%>" class="style12">Delete</a></div></td>
		  <td bordercolor="#663300" bgcolor="#99BBB7"><div align="center"><a href="Resume.jsp?uid=<%=uid%>"><strong>View</strong></a></div></td>
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