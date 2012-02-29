<%@page  import="java.sql.*" %>
<%@page import="java.io.*,java.sql.*,java.util.*,java.util.Vector;"%>
<%@page contentType="text/html" pageEncoding="UTF-8"%>
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN"
   "http://www.w3.org/TR/html4/loose.dtd">

<html>
    <head>
        <meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
        <title>www.jobconsultants.com</title>
    </head>
    <body>
        <table>
    
<% try      
      {

      String qno=request.getParameter("qno");
       String jid=request.getParameter("jid");
        String eid=request.getParameter("eid");
      out.println(qno);
           Class.forName("sun.jdbc.odbc.JdbcOdbcDriver");
           Connection con=DriverManager.getConnection("jdbc:odbc:OnlineExam");
           Statement st=con.createStatement();
     st.executeUpdate("Delete from Question where QuestionNo="+qno+" and ExamID='" + eid +
             "' and JobID='" + jid + "'");
       response.sendRedirect("ViewQues.jsp?jobid=" + jid + "&examid=" + eid);
   }//try
      
    catch(Exception e)
     {
        out.println(e);        
     }
%>
    </table>
    </body>
</html>
