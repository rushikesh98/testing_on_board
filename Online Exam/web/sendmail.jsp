<%-- 
    Document   : Send mail
    
--%>

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
                font-size: 24px;
                font-weight: bold;
                color: #993300;
            }
            .style2 {
                color: #000000;
                font-weight: bold;
            }
            -->
        </style>
        <script language="javascript">
            function check()
            {
				var a= document.form1.jobid.value;
				var b= document.form1.vacancies.value;
				var c= document.form1.salary.value;
				
                if(document.form1.jobid.value=="")
                {
                    alert("Enter User Name");
                    document.form1.jobid.focus();
                    return false;
                }
				
                if(document.form1.post.value=="")
                {
                    alert("Enter Sender E-mail Address");
                    document.form1.post.focus();
                    return false;
                }
                if(document.form1.criteria.value=="")
                {
                    alert("Enter Receiver E-mail Address");
                    document.form1.criteria.focus();
                    return false;
                }
                if(document.form1.vacancies.value=="")
                {
                    alert("Enter Subject for Sending the Mail");
                    document.form1.vacancies.focus();
                    return false;
                }
				
                if(document.form1.salary.value=="")
                {
                    alert("Enter the Content for sending");
                    document.form1.salary.focus();
                    return false;
                }
		        return true;
            }

        </script>
    </head>
    <body style="margin:0px" bgcolor="#FFFFFF">
        <table width="100%" border="1" bgcolor="#FFFFFF">
            <tr>
                <td height="107" colspan="2"><jsp:include page="CandidateHeader.jsp"/></td>
            </tr>
            <tr>
                <td width="1" height="512"><jsp:include page="Candidatemenu.jsp"/></td>
                <td width="961" bgcolor="#6699CC">
                    <form name="form1" method="post" action="sendmailcompany.jsp">
                        <table width="517" height="334" border="1" align="center" bgcolor="#FFCCCC">
                            <tr bordercolor="#0099FF">
                                <td height="30" colspan="2" bgcolor="#E3EAF4"><div align="center" class="style1">Send E-mail To Company </div></td>
                            </tr>
                            <tr>
                                <td width="160" height="26" bordercolor="#0099FF" bgcolor="#E3EAF4"><div align="left" class="style2">User Name</div></td>
                                <td width="213" bordercolor="#0099FF" bgcolor="#E3EAF4">
                                    <label>
                                        <input name="jobid" id="jobid"type="text" size="30">
                                </label>            </td>
                            </tr>
                            <tr>
                                <td height="26" bordercolor="#0099FF" bgcolor="#E3EAF4"><div align="left" class="style2"> Enter the User E-mail </div></td>
                                <td bordercolor="#0099FF" bgcolor="#E3EAF4"><input name="post" name="post" type="text" size="30"></td>
                            </tr>
                            <tr>
                                <td height="26" bordercolor="#0099FF" bgcolor="#E3EAF4"><div align="left" class="style2"> Enter the Company E-mail</div></td>
                                <td bordercolor="#0099FF" bgcolor="#E3EAF4"><input name="criteria" name="criteria" type="text" size="30"></td>
                            </tr>
                            <tr>
                                <td height="26" bordercolor="#0099FF" bgcolor="#E3EAF4"><div align="left" class="style2">Subject </div></td>
                                <td bordercolor="#0099FF" bgcolor="#E3EAF4"><input name="vacancies" name="vacancies" type="text" size="30"></td>
                            </tr>
                            <tr>
                                <td height="26" bordercolor="#0099FF" bgcolor="#E3EAF4"><div align="left" class="style2">Content</div></td>
                                <td bordercolor="#0099FF" bgcolor="#E3EAF4"><input name="salary" name="salary" type="TextArea" size="30"></td>
                            </tr>
                           
                            <tr bordercolor="#0099FF">
                                <td height="28" colspan="2" bgcolor="#E3EAF4">
                                    <div align="center">
                                        <input type="Button" name="Submit" value="Send the E-mail" onClick="return check()">
                                </div>           </td>
                            </tr>
                        </table>
                    </form>
                    <h2>&nbsp;</h2>
                </td>
            </tr>
        </table>
    </body>
</html>