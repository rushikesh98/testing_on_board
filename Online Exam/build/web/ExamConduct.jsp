
<%@page import="java.io.*,java.sql.*;"session="true"%>
<%@page contentType="text/html" pageEncoding="UTF-8"%>
<%
        Connection con = null;
        Statement st = null, st2 = null;
        ResultSet rs = null;
        String sql = null;
        try {
            Class.forName("sun.jdbc.odbc.JdbcOdbcDriver");
            con = DriverManager.getConnection("jdbc:odbc:OnlineExam");
            st = con.createStatement();
        } catch (Exception ex) {
            out.println("Error: " + ex.toString());
        }
%>



<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN"
"http://www.w3.org/TR/html4/loose.dtd">

<html>
    <head>
        <meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
        <title>JSP Page</title>
        <style type="text/css">
            <!--
            .style1 {
                font-size: 18px;
                font-weight: bold;
                color: #FF0000;
            }
            .style7 {color: #006666; font-weight: bold; }
            .style14 {color: #006699}
            -->
        </style>
    </head>
    <body style="margin:0px" bgcolor="#FFFFFF">
        <table width="100%" border="1" bgcolor="#FFFFFF">
            <tr>
                <td height="107" colspan="2"><jsp:include page="CandidateHeader.jsp"/></td>
            </tr>
            <tr>
                <td width="1" height="512"><jsp:include page="Candidatemenu.jsp"/></td>
                <td width="961" bgcolor="#D8E7E7" valign="top">
                    <table width="304" height="243" border="0" align="center">
                        <tr>
                            <td colspan="2"><div align="center" class="style1">
                                    <p>Exam</p>
                                    <p>&nbsp;</p>
                            </div></td>
                        </tr>
                        <tr>


                        <%
        String jid = request.getParameter("jobid");
        try {
            if (jid == null) {
                        %>
                        <form name="exam1" action ="ExamConduct.jsp">
                            <td width="167" height="42"><span class="style7">Select a JobID </span></td>
                            <td width="112">

                                <select name="jobid">
                                    <%
                                rs = st.executeQuery("select * from JobDetails");
                                String s1 = null;
                                // String s2=null;
                                while (rs.next()) {
                                    s1 = rs.getString("JobId");
                                    //    s2=rs.getString("Post");
                                    out.println("<option value='" + s1 + "'>" + s1 + "</option>");
                                }//while

                                    %>
                                </select>
                            </td>
                            <tr><td> <div align="center">
                                        <input type="submit" value="OK ">
                            </div></td></tr>
                        </form>
                        <%
                            }//if
                            else {
                                jid = request.getParameter("jobid");
                                sql = "select Post from Exam where JobID='" + jid + "'";
                                rs = st.executeQuery(sql);
                                String post = null;
                                if (rs.next()) {
                                    post = rs.getString("Post");
                                }
                                rs.close();


                        %> <form name="examSelection" action ="11.jsp">
                            <tr>
                                <td><span class="style7">Post Name :</span></td>
                                <td><%=post%></td>
                                <td width="11">&nbsp;</td>
                            </tr>

                            <tr>
                                <td height="30" class="style7">&nbsp;</td>
                                <td>&nbsp;</td>
                                <td>&nbsp;</td>
                            </tr>
                            <tr>
                                <td height="30" class="style7">Select an Exam</td>
                                <td><select name="examid">

                                        <%
                                sql = "select ExamID,ExamName from Exam where JobID='" + jid + "'";
                                rs = st.executeQuery(sql);
                                String exname = null;
                                String exid = null;
                                while (rs.next()) {
                                    exid = rs.getString("ExamID");
                                    exname = rs.getString("ExamName");
                                    out.println("<option value='" + exid + "'>" + exname + "</option>");
                                }
                                        %>
                                    </select>
                                </td>
                                <td>&nbsp;</td>
                            </tr>
                            <tr>
                                <td class="style7"><div align="center">
                                        <input type="submit" value="OK ">
                                </div></td>
                            </tr>
                        </form>
                    </table>
                    <p>&nbsp;</p>

                    <%

                session.setAttribute("jobid", request.getParameter("jobid"));
                session.setAttribute("post", post);
            }//else
        }//try
        catch (Exception e) {
            out.println(e);
        }
                    %>

                </td>

            </tr>
        </table>
    </body>
</html>
