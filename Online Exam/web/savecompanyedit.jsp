<%-- 
    Document   : savecompanyedit
   
--%>
<%@page import="java.io.*,java.sql.*;"%>
<%@page contentType="text/html" pageEncoding="UTF-8"%>
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN"
"http://www.w3.org/TR/html4/loose.dtd">

<html>
    <head>
        <meta http-equiv="Content-Type" content="text/html; charset=utf-8">
        <title>www.jobconsultants.com</title>
        <style type="text/css">
            <!--
            body {
                background-color: #EFF0DD;
            }
            -->
    </style></head>
    <body >
        <p>
            <% try {
            String c1 = (String) session.getAttribute("s1");
            // session.getAttribute("c1", c1);
            String c2 = (String) session.getAttribute("s2");
            Class.forName("sun.jdbc.odbc.JdbcOdbcDriver");
            Connection con = DriverManager.getConnection("jdbc:odbc:OnlineExam");
            Statement st = con.createStatement();
            String sql = "Update Com set CName ='" + request.getParameter("comname") + "',CAddress ='" +
                    request.getParameter("comaddress") + "',CPost ='" + request.getParameter("compost") +
                    "',CCriteria ='" + request.getParameter("comcriteria") + "',CEmail ='" + request.getParameter("comemail") +
                    "',CContact ='" + request.getParameter("comcontact") + "',CCutoff ='" + request.getParameter("comcutoff") +
                    "' where CUsername ='" + c1 + "'";
            st.executeUpdate(sql);
            %>
            <font color="red" size="5">
                <%out.println("You have Successfully Edited!!!");%>
            </font>
            <%
        } catch (Exception e) {
            out.println(e);
        }

            %>
        </p>
        <p><a href="CompanyHomePage.jsp"><strong>Go forward </strong></a></p>
    </body>
</html>
