<%-- 
    Document   : savejoedit
   
--%>

<%@page  import="java.sql.*" %>
<%@page import="java.io.*,java.sql.*,java.util.*,java.util.Vector;"%>
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
                background-color: #E2EFD6;
            }
            -->
    </style></head>
    <body>
        <table>
            <% try {
            String jid = request.getParameter("jobid");
            Class.forName("sun.jdbc.odbc.JdbcOdbcDriver");
            Connection con = DriverManager.getConnection("jdbc:odbc:OnlineExam");
            Statement st = con.createStatement();
            st.executeUpdate("Update JobDetails set Post='" + request.getParameter("post") + "',Criteria='" + request.getParameter("criteria") + "',NoOfVacancies='" + request.getParameter("vacancies") + "',Salary='" + request.getParameter("salary") + "',LastDate='" + request.getParameter("lastdate") + "' where JobId='" + jid + "'");
            %>
            <font color="red" size="5"> <%out.println("You have Successfully Edited!!!");%></font>
            <%   } catch (Exception e) {
            out.println(e);
        }

            %>
        </table>
        <p>&nbsp;</p>
        <p><a href="CompanyHomePage.jsp"><strong>Goto Home</strong></a> </p>
    </body>
</html>
