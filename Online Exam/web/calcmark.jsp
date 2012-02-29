<form id="form1" name="form1" method="post" action="calcmark.jsp">
    <input name="OK" type="submit" id="OK" value="Submit" />
</form>
<%@page contentType="text/html" pageEncoding="UTF-8" import="java.sql.*" session="true" %>
<%
        Connection con;
        Statement st;
        ResultSet rs;
        String sql;

        int mark = Integer.parseInt(session.getAttribute("mark").toString());
        String ch = request.getParameter("ch");
        String ans = request.getParameter("ans");
        int qno = Integer.parseInt(request.getParameter("qno"));
        if (ch != null) {
//out.println("Ch=" + ch + "<br> Ans=" + ans + "<br>Qno=" + qno + " Mark=" + mark);
            ch = ch.trim();
            ans = ans.trim();
            if (ch.equalsIgnoreCase(ans)) {
                mark++;
                session.setAttribute("mark", mark);

            }
        }
        if (qno < 10) {
            response.sendRedirect("dispqst.jsp?qno=" + (qno + 1));
        } else {
            try {
                String studentid = session.getAttribute("s1").toString();
                sql = "delete from Temp_Data where StudID='" + studentid + "'";
                Class.forName("sun.jdbc.odbc.JdbcOdbcDriver");
                con = DriverManager.getConnection("jdbc:odbc:OnlineExam");
                st = con.createStatement();
                st.executeUpdate(sql);

                String examID = session.getAttribute("examid").toString();
                String jobID = session.getAttribute("jobid").toString();
                mark=Integer.parseInt(session.getAttribute("mark").toString());
                sql="Insert into Result values('" + studentid + "','" +  examID + "','" + jobID + "'," + mark + ")";
                st.executeUpdate(sql);
                response.sendRedirect("dispresult.jsp");
            } catch (Exception ex) {
                out.println(ex.toString());
            }

        }

%>