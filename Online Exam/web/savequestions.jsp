<%-- 
    Document   : savequestions
    
--%>
<%@page import="java.io.*,java.sql.*"%>
<%@page contentType="text/html" pageEncoding="UTF-8"%>
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN"
   "http://www.w3.org/TR/html4/loose.dtd">

<html>
    <head>
        <meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
        <title>www.jobconsultants.com</title>
    </head>
    <body background="wpa552ad7e[1].png">
         <% try
          {
         
           Class.forName("sun.jdbc.odbc.JdbcOdbcDriver");
           Connection con=DriverManager.getConnection("jdbc:odbc:OnlineExam");
           Statement st=con.createStatement();
       //   Statement st2=con.createStatement();
          
         // String stat="c";
         // st2.executeUpdate("insert into Login values('"+request.getParameter("comusername")+"','"+request.getParameter("compassword")+"','"+stat+"')");
          st.executeUpdate("insert into Question values('"+request.getParameter("jobid")+"','"+request.getParameter("examid")+"','"+request.getParameter("Qno")+"','"+request.getParameter("question")+"','"+request.getParameter("option1")+"','"+request.getParameter("option2")+"','"+request.getParameter("option3")+"','"+request.getParameter("option4")+"','"+request.getParameter("answer")+"')");
          
          
           st.close();
          // st2.close();
           
             }
       
        
           catch(Exception e)
                   {
                   }
        
           %>
            <font color="red" size="5"><br>
               <%out.println("Questions are added!!!");%>
               <br>
             
          </font>
           <p>&nbsp;</p>
        <p><a href="AddQuestions.jsp"><strong>Go Back</strong></a> </p>
    </body>
</html>
