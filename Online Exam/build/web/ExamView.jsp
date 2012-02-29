<%-- 
    Document   : ExamView
    Created on : Nov 18, 2008, 2:27:28 PM
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
                font-size: 18px;
                font-weight: bold;
                color: #FF0000;
            }
            .style7 {color: #006666; font-weight: bold; }
            .style9 {color: #663366; font-weight: bold; }
            -->
        </style>
    </head>
    <body style="margin:0px" bgcolor="#FFFFFF">
        <table width="100%" border="1" bgcolor="#FFFFFF" >
            <tr>
                <td height="23" colspan="2"><jsp:include page="CompanyHeader.jsp"/></td>
            </tr>
            <tr>
                <td width="1" height="537"><jsp:include page="Companymenu.html"/></td>
                <td width="961" bgcolor="#EDE0E2" valign="top"><form name="form1" action="ExamView.jsp">
                        <table width="277" height="128" border="0" align="center"  >
                            <tr>
                                <td height="49" colspan="2"><div align="center" class="style1">
                                        Exam View
                                </div></td>
                            </tr>
                            <%
        String jid = request.getParameter("jobid");
        //out.print(jid);
        try {
            if (jid == null) {
                Class.forName("sun.jdbc.odbc.JdbcOdbcDriver");
                Connection con = DriverManager.getConnection("jdbc:odbc:OnlineExam");
                Statement st = con.createStatement();
                            %>
                            <tr>
                                <td width="135" height="42"><span class="style7">Select a JobID </span></td>
                                <td width="132">
                                    <label>
                                        <select name="jobid">
                                            <%      ResultSet rs = st.executeQuery("select * from JobDetails");
                            String s1 = null;

                            while (rs.next()) {
                                s1 = rs.getString("JobId");


                                            %>
                                            <option value="<%=s1%>"><%=s1%></option>
                                            <%     }
                                            %>
                                        </select>
                                    </label>
                                </td>
                            </tr>
                            <tr><td> <input type="submit" value="Show Details"> </td></tr>
                        </table>
                    </form>

                    <%
                            } else {
                                jid = request.getParameter("jobid");
                                Class.forName("sun.jdbc.odbc.JdbcOdbcDriver");
                                Connection con = DriverManager.getConnection("jdbc:odbc:OnlineExam");
                                Statement st = con.createStatement();

                                ResultSet rs = st.executeQuery("select * from JobDetails where JobId='" + jid + "'");
                                String s2 = null;
                                while (rs.next()) {
                                    s2 = rs.getString("Post");
                                }
                    %>
                    <table width="549" height="54" border="1" align="center" style="vertical-align:top">

                        <tr>
                            <td width="98" height="23" bordercolor="#344631" bgcolor="#F8E0E0"><div align="center" class="style9">Exam ID </div></td>
                            <td width="139" bordercolor="#344631" bgcolor="#F8E0E0"><div align="center" class="style9">Exam Name </div></td>
                            <td width="104" bordercolor="#344631" bgcolor="#F8E0E0"><div align="center" class="style9">Cutoff % </div></td>
                            <td width="89" bordercolor="#344631" bgcolor="#F8E0E0"><div align="center" class="style9">Edit</div></td>
                            <td width="85" bordercolor="#344631" bgcolor="#F8E0E0"><div align="center" class="style9">Delete</div></td>
                        </tr>

                        <%    Statement st2 = con.createStatement();
                                ResultSet rs2 = st2.executeQuery("select * from Exam where JobId='" + jid + "'");

                                while (rs2.next()) {
                                    String eid = rs2.getString("ExamID");
                                    String ename = rs2.getString("ExamName");
                                    String ecut = rs2.getString("Cutoff");
                        %>
                        <tr valign="top">
                            <td height="23" bordercolor="#344631" bgcolor="#F8E0E0"><%=eid%></td>
                            <td bordercolor="#344631" bgcolor="#F8E0E0"><%=ename%></td>
                            <td bordercolor="#344631" bgcolor="#F8E0E0"><%=ecut%></td>
                            <td bordercolor="#344631" bgcolor="#F8E0E0"><a href="EditExam.jsp?eid=<%=eid%>&jid=<%=jid%>">Edit</a></td>
                            <td bordercolor="#344631" bgcolor="#F8E0E0"><a href="DeleteExam.jsp?eid=<%=eid%>">Delete</a></td>
                        </tr>
                        <%
                }
            }
        } catch (Exception e) {
            out.println(e);
        }
                        %>
                    </table>

                </td>
            </tr>
        </table>
    </body>
</html> 