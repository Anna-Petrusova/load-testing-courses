Action()
{
lr_start_transaction("6_Task_Register");
	lr_start_transaction("open_homepage");

	web_reg_save_param_regexp(
	"ParamName=userSession",
	"RegExp=name=\"userSession\" value=\"(.+?)\"\/>",
	SEARCH_FILTERS,
	"Scope=BODY",
	LAST);
	
	web_reg_find("Search=Body",
		"Text=<title>Web Tours</title>",
		LAST);
	
	web_url("WebTours", 
		"URL=http://localhost:1080/WebTours/", 
		"TargetFrame=", 
		"Resource=0", 
		"RecContentType=text/html", 
		"Referer=", 
		"Snapshot=t1.inf", 
		"Mode=HTML", 
		LAST);
	lr_end_transaction("open_homepage", LR_AUTO);
	
	lr_start_transaction("open_signuppage");
	
	web_reg_find("Search=Body",
	"Text=<title>User Information</title>",
	LAST);
		
	web_url("sign up now", 
		"URL=http://localhost:1080/cgi-bin/login.pl?username=&password=&getInfo=true", 
		"TargetFrame=body", 
		"Resource=0", 
		"RecContentType=text/html", 
		"Referer=http://localhost:1080/WebTours/home.html", 
		"Snapshot=t2.inf", 
		"Mode=HTML", 
		LAST);
	lr_end_transaction("open_signuppage", LR_AUTO);

	lr_think_time(21);

	lr_start_transaction("add_register_data");

	
	web_reg_find("Search=Body",
	"Text=Thank you, <b>{login}</b>, for registering",
	LAST);
	
	web_submit_data("login.pl", 
		"Action=http://localhost:1080/cgi-bin/login.pl", 
		"Method=POST", 
		"TargetFrame=info", 
		"RecContentType=text/html", 
		"Referer=http://localhost:1080/cgi-bin/login.pl?username=&password=&getInfo=true", 
		"Snapshot=t3.inf", 
		"Mode=HTML", 
		ITEMDATA, 
		"Name=username", "Value={login}", ENDITEM, 
		"Name=password", "Value={password}", ENDITEM, 
		"Name=passwordConfirm", "Value={password}", ENDITEM, 
		"Name=firstName", "Value={firstName}", ENDITEM, 
		"Name=lastName", "Value={lastName}", ENDITEM, 
		"Name=address1", "Value={address1}", ENDITEM, 
		"Name=address2", "Value={address2}", ENDITEM, 
		"Name=register.x", "Value=kg8jg", ENDITEM, 
		"Name=register.y", "Value=kg8jg", ENDITEM, 
		LAST);
	lr_end_transaction("add_register_data", LR_AUTO);

	
	lr_start_transaction("open_intro_page");
	
	web_reg_find("Search=Body",
	"Text=Welcome, <b>{login}</b>, to the Web Tours reservation pages",
	LAST);
	
	web_url("button_next.gif", 
		"URL=http://localhost:1080/cgi-bin/welcome.pl?page=menus", 
		"TargetFrame=body", 
		"Resource=0", 
		"RecContentType=text/html", 
		"Referer=http://localhost:1080/cgi-bin/login.pl", 
		"Snapshot=t4.inf", 
		"Mode=HTML", 
		LAST);
	lr_end_transaction("open_intro_page", LR_AUTO);

	lr_think_time(15);
	
	lr_start_transaction("logout");

	web_reg_find("Search=Body",
		"Text=<title>Web Tours</title>",
		LAST);
	
	web_url("SignOff Button", 
		"URL=http://localhost:1080/cgi-bin/welcome.pl?signOff=1", 
		"TargetFrame=body", 
		"Resource=0", 
		"RecContentType=text/html", 
		"Referer=http://localhost:1080/cgi-bin/nav.pl?page=menu&in=home", 
		"Snapshot=t5.inf", 
		"Mode=HTML", 
		LAST);
	lr_end_transaction("logout", LR_AUTO);
lr_end_transaction("6_Task_Register", LR_AUTO);

	return 0;
}