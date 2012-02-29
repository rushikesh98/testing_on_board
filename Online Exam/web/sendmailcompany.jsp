<%-- 
    Document   : savecompanydata
    
--%>
<%@page import="java.io.*,java.sql.*;"%>
<%@page contentType="text/html" pageEncoding="UTF-8"%>
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN"
   "http://www.w3.org/TR/html4/loose.dtd">

<html>
    <head>
        <meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
        <title>www.jobconsultants.com</title>
    </head>
    <body bgcolor="#E2C2CB">
        <% try
          {
         
           Class.forName("sun.jdbc.odbc.JdbcOdbcDriver");
           Connection con=DriverManager.getConnection("jdbc:odbc:OnlineExam");
          
            Statement st=con.createStatement();
          Statement st2=con.createStatement();
          
          String stat="c";
 st.executeUpdate("insert into smail(username,smailid,subject,content) values('"+request.getParameter("jobid")+"','"+request.getParameter("post")+"','"+request.getParameter("criteria")+"','"+request.getParameter("vacancies")+"','"+request.getParameter("salary")+"')");
          
          st.close();
          st2.close();
          
             }
       
        
           catch(Exception e)
                   {
               out.println(e);
                   }
        
           %>
           <font color="red" size="5"><br>
               <%out.println("Mail send  Successfull!!!");%>
               <br>
               <%out.println("You will be activated soon... ");%>
               <br>
                <p><a href="CandidateHomePage.jsp">Go Forward</a></p>
          </font>
    </body>
</html>
