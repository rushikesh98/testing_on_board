<%-- 
    Document   : EditComProfile
    Created on : Nov 15, 2008, 1:16:18 PM
    Author     : user
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
            color: #CC6633;
            font-weight: bold;
            font-size: 18px;
        }
        .style2 {color: #FFFFFF}
        -->
    </style>
    <script language="javascript">
			function check()
			{
				var a=document.form1.comname.value;
           		var c=document.form1.comemail.value;
            	var d=document.form1.comcontact.value;
            	var e=document.form1.comcutoff.value;
				if(document.form1.comname.value == "")
				{
					alert("Enter Company Name");
                	document.form1.comname.focus();
                	return false;
				}
				else if(parseInt(a))
           		 {
                alert("Company Name cannot be integer");
                document.form1.comname.focus();
                return false;
           		 }
				if(document.form1.comaddress.value == "")
				{
					alert("Enter Company Address");
                	document.form1.comaddress.focus();
                	return false;
				}
				if(document.form1.compost.value == "")
				{
					alert("Enter Post");
                	document.form1.compost.focus();
                	return false;
				}
				if(document.form1.comcriteria.value == "")
				{
					alert("Enter Criteria of the Company");
                	document.form1.comcriteria.focus();
                	return false;
				}
				if(document.form1.comemail.value == "")
				{
					alert("Enter Company Email");
                	document.form1.comemail.focus();
                	return false;
				}
				else if(c.indexOf('@')==-1)
         	   {
                alert("Enter Email Correctly");
                document.form1.comemail.focus();
                return false;
          		}
           		 else if(c.indexOf('.')==-1)
           		 {
                alert("Enter Email Correctly");
                document.form1.comemail.focus();
                return false;
          		  }
				if(document.form1.comcontact.value == "")
				{
					alert("Enter Company Contact no:");
                	document.form1.comcontact.focus();
                	return false;
				}
				else if(!parseInt(d))
            	{
                alert("Enter integer");
                document.form1.comcontact.focus();
                return false;
            	}
            	else if(((document.form1.comcontact.value.length)<6) || ((document.form1.comcontact.value.length)>11))
            	{
                alert("Phone no should contain atleast 6 integers ");
                document.form1.comcontact.focus();
                return false;
            	}
				if(document.form1.comcutoff.value == "")
				{
					alert("Enter Company Cutoff mark");
                	document.form1.comcutoff.focus();
                	return false;
				}
				 else if(!parseInt(e))
            	{
                alert("Enter integer");
                document.form1.comcutoff.focus();
                return false;
            	}
           	 	else if(parseInt(e)<0||parseInt(e)>100)
            	{
                alert("Enter cutoff in %");
                document.form1.comcutoff.focus();
                return false;
            	}
				return true;
				}
	</script>

    <body style="margin:0px" bgcolor="#FFFFFF">
        <table width="100%" border="1" bgcolor="#FFFFFF">
            <tr>
                <td height="107" colspan="2"><jsp:include page="CompanyHeader.jsp"/></td>
            </tr>
            <tr>
                <td width="1" height="512"><jsp:include page="Companymenu.html"/></td>
                <td width="961" bgcolor="#E7E8EB" valign="top">
                    <span class="style2">
                        <%
        //out.println(c1);
        try {

            //

            String c1 = (String) session.getAttribute("s1");
            Class.forName("sun.jdbc.odbc.JdbcOdbcDriver");
            Connection con = DriverManager.getConnection("jdbc:odbc:OnlineExam");
            Statement st = con.createStatement();


            ResultSet rs = st.executeQuery("select CName,CAddress,CPost,CCriteria,CEmail,CContact,CCutoff from Com where CUsername='" + c1 + "'");
            String a = null;
            String b = null;
            String c = null;
            String d = null;
            String e = null;
            String f = null;
            String g = null;
            while (rs.next())
            {
                a = rs.getString("CName");
                b = rs.getString("CAddress");
                c = rs.getString("CPost");
                d = rs.getString("CCriteria");
                e = rs.getString("CEmail");
                f = rs.getString("CContact");
                g = rs.getString("CCutoff");

                        %>
                    </span>
                    <div class="Reg" align="center">
                        <form method="POST" action="savecompanyedit.jsp" name="form1" onSubmit="return check()">
                            <table width =66% border=0 cellpadding=3 cellspacing=0 bordercolor="#FFFFFF" >
                                <tr bgcolor="" width=100%>
                                    <td colspan="2" bgcolor="#F5E7DC">
                                    <div align="center" class="style5 style1">Edit Company Profile </div>                </td>
                                </tr>
                                <tr bgcolor="" width=100%>
                                    <td width="46%" bgcolor="#FFFFFF">
                                    <div align="center"><font SIZE="2" COLOR="black" face =arial >Name</font></div></td>
                                    <td width="54%" bgcolor="#FFFFFF">
                                        <div align="left">
                                            <input TYPE="text" NAME="comname" id="comname" size=30 maxlength=30 value="<%=a%>">
                                    </div>	</td>
                                </tr>
                                <tr bgcolor =#FEF7E9>
                                    <td bgcolor="#D9DEE6">
                                    <div align="center"><font SIZE="2" COLOR="black" face =arial>Address</font></div></td>
                                    <td bgcolor="#D9DEE6">
                                        <div align="left">
                                            <input NAME="comaddress" TYPE="text" id="comaddress" size=30 maxlength=50 value="<%=b%>">
                                    </div>	 </td>
                                </tr>
                                <tr bgcolor =#FEF7E9>
                                    <td bgcolor="#F5E7DC">
                                    <div align="center"><font SIZE="2" COLOR="black" face =arial>Post</font></div></td>
                                    <td bgcolor="#F5E7DC">
                                        <div align="left">
                                            <input NAME="compost" TYPE="text" id="compost" size=30  maxlength=50 value="<%=c%>">
                                    </div>	 </td>
                                </tr>
                                <tr bgcolor =#FEF7E9>
                                    <td bgcolor="#D9DEE6">
                                    <div align="center"><font SIZE="2" COLOR="black" face =arial>Criteria</font></div></td>
                                    <td bgcolor="#D9DEE6">
                                        <div align="left">
                                            <input NAME="comcriteria" TYPE="text" id="comcriteria" size=30  maxlength=50 value="<%=d%>">
                                    </div>	</td>
                                </tr>
                                <tr  bgcolor =#FEF7E9>
                                    <td bgcolor="#F5E7DC">
                                    <div align="center"><font SIZE="2" COLOR="black" face =arial>Email_id</font></div></td>
                                    <td bgcolor="#F5E7DC">
                                        <div align="left">
                                            <input TYPE="text" NAME="comemail" id="comemail" size=30 maxlength=30 value="<%=e%>">
                                    </div>	   </td>
                                </tr>
                                <tr>
                                    <td bgcolor="#FFFFFF">
                                    <div align="center"><font SIZE="2" COLOR="black" face =arial>Contact No:</font></div></td>
                                    <td bgcolor="#FFFFFF">
                                        <div align="left">
                                            <input TYPE="text" NAME="comcontact" id="comcontact"size=20 maxlength=30 value="<%=f%>">
                                    &nbsp;                 <font SIZE="2" COLOR="#CC0000" face =arial>(STD-No.)</font>		   </div>	   </td>
                                </tr>
                                <tr  bgcolor =#FEF7E9>
                                    <td bgcolor="#D9DEE6">
                                    <div align="center"><font SIZE="2" COLOR="black" face =arial>CutOff %</font></div></td>
                                    <td bgcolor="#D9DEE6">
                                        <div align="left">
                                            <input TYPE="text" NAME="comcutoff" id="comcutoff" size=30 maxlength=30 value="<%=g%>">
                                    </div>	   </td>
                                </tr>
                                <tr>
                                    <td height="49" colspan=2 bgcolor="#F5E7DC" >
                                        <div align="center"><br>
                                            <input type="reset" />
                                            &nbsp;&nbsp;<input TYPE="submit" value="Edit" >
                                    </div>	    </td>
                                </tr>
                            </table>
                        </form>
                    </div>

                    <% }//while

        } catch (Exception e) {
            out.println(e.toString());
        }
                    %>
                </td>
            </tr>
        </table>
    </body>
</html>

