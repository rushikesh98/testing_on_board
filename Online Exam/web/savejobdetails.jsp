<%-- 
    Document   : savejobdetails
    
--%>
<%@page import="java.io.*,java.sql.*;"%>
<%@page contentType="text/html" pageEncoding="UTF-8"%>
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN"
   "http://www.w3.org/TR/html4/loose.dtd">

<html>
    <head>
        <meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
        <title>www.jobconsultants.com</title>
        <style type="text/css">
<!--
.style1 {
	font-size: 5px;
	font-weight: bold;
}
.style2 {
	font-size: 10pt
}
-->
        </style>
</head>
    <body background="wpa552ad7e[1].png">
         <% try
          {
         
           Class.forName("sun.jdbc.odbc.JdbcOdbcDriver");
           Connection con=DriverManager.getConnection("jdbc:odbc:OnlineExam");
          
            Statement st=con.createStatement();
          Statement st2=con.createStatement();
          
         // String stat="c";
         // st2.executeUpdate("insert into Login values('"+request.getParameter("comusername")+"','"+request.getParameter("compassword")+"','"+stat+"')");
          st.executeUpdate("insert into JobDetails values('"+request.getParameter("jobid")+"','"+request.getParameter("post")+"','"+request.getParameter("criteria")+"','"+request.getParameter("vacancies")+"','"+request.getParameter("salary")+"','"+request.getParameter("lastdate")+"')");
          st2.executeUpdate("insert into Exam values('"+request.getParameter("jobid")+"','"+request.getParameter("post")+"')");
          
           st.close();
           st2.close();
           
             }
       
        
           catch(Exception e)
                   {
                   }
        
           %>
            <font color="red" size="5"><br>
               <%out.println("Data submitted successfully!!!");%>
               <br>
             <p>&nbsp;</p>
        </font>
    <p class="style1"><font color="red"><a href="CompanyHomePage.jsp" class="style2">Goto Home</a></font> </p>
</body>
</html>
