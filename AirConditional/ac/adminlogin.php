<!DOCTYPE html>
<html xmlns="http://www.w3.org/1999/xhtml">

<head>
	<meta http-equiv="Content-Type" content="text/html; charset=UTF-8" />
	
	<title>Login Page</title>
	
	<link rel="stylesheet" type="text/css" href="login.css" />
</head>

<script type="text/javascript">
	function validateLogin(){
		if (!(document.forms["login"]["username"].value == "administrator" && document.forms["login"]["password"].value == "iforgot")){
			alert("用户名或密码错误！");
			return false;
		}
	}
</script>

<body>

	<form name="login" id="login-form" method="post" action="serverv2.0/server.php" onsubmit="return validateLogin()">
		<fieldset>
            
			<br />
			<br />
            
            <P>welcome to login</p>
            
            <br />
            <br />
            
			<label for="login">用户名</label>
			<input type="text" id="username" name="username"/>
			
            <br />
            
			<label for="password"> 密码 </label>
			<input type="password" id="password" name="password"/>
			
			<br />
            <table>
                <tr>
            <td><input type="reset" style="margin: 0 0 0 100px;" class="button" name="reset" value="重置"/></td>
			<td><input type="submit" style="margin: 0 0 0 50px;" class="button" name="login" value="登陆"/></td>
            </tr></table>
            
            <br />
            <br />
		</fieldset>
	</form>

</body>

</html>