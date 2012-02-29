<%@ page isErrorPage="true" import="java.io.*" %>
<html>
<head>
	<title>Exceptional even occurred</title>
</head>
<body>
<%-- Exception Handler --%>
<%= exception.toString() %><br>
<%
out.println("<!--");
StringWriter sw = new StringWriter();
PrintWriter pw = new PrintWriter(sw);
exception.printStackTrace(pw);
out.print(sw);
sw.close();
pw.close();
out.println("-->");
%>
</body>
</html>
