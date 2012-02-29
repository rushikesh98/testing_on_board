<%-- 
    Document   : LoginSession
    Created on : Nov 11, 2008, 2:39:56 PM
    Author     : user
--%>

<%@page contentType="text/html" pageEncoding="UTF-8"%>
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN"
   "http://www.w3.org/TR/html4/loose.dtd">

<html>
    <head>
        <meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
        <title>www.CarrierMithr.com</title>
    </head>
    <body>
         <% try
             {
                HttpSession s2=request.getSession();
                HttpSession s3=request.getSession();
                s2.setAttribute("username", request.getParameter("username")); 
                s3.setAttribute("password", request.getParameter("password")); 
                response.sendRedirect("Valid.jsp");  
             }  
           catch(Exception e) 
                   {
                     out.println(e);
                   }       
         %>
    </body>
</html>
