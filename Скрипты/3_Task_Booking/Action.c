Action()
{
	lr_start_transaction("3_Task_Booking");
	
	
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
		"Snapshot=t22.inf", 
		"Mode=HTML", 
		LAST);
	
	lr_end_transaction("open_homepage", LR_AUTO);

	lr_think_time(7);

	lr_start_transaction("login");
	
	web_reg_find("Search=Body",
		"Text=Welcome, <b>{login}</b>, to the Web Tours",
		LAST);

	web_submit_data("login.pl",
		"Action=http://localhost:1080/cgi-bin/login.pl",
		"Method=POST",
		"TargetFrame=body",
		"RecContentType=text/html",
		"Referer=http://localhost:1080/cgi-bin/nav.pl?in=home",
		"Snapshot=t23.inf",
		"Mode=HTML",
		ITEMDATA,
		"Name=userSession", "Value={userSession}", ENDITEM,
		"Name=username", "Value={login}", ENDITEM,
		"Name=password", "Value={password}", ENDITEM,
		"Name=JSFormSubmit", "Value=off", ENDITEM,
		"Name=login.x", "Value=59", ENDITEM,
		"Name=login.y", "Value=1", ENDITEM,
		LAST);
	
	lr_end_transaction("login", LR_AUTO);
	
	lr_start_transaction("open_Flights_page");
	
	web_reg_find("Search=Body",
		"Text=<title>Flight Selections</title>",
		LAST);
	
	//////////берем список городов со страницы
		web_reg_save_param_regexp(
		"ParamName=cities",
		"RegExp=value=\"(.+?)\">(.+?)<\/option>",
		"Group=1",
		//"NotFound=WARNING",
		"Ordinal=ALL",	
		SEARCH_FILTERS,
		"Scope=BODY",
		LAST);
	
		web_url("Search Flights Button", 
		"URL=http://localhost:1080/cgi-bin/welcome.pl?page=search", 
		"TargetFrame=body", 
		"Resource=0", 
		"RecContentType=text/html", 
		"Referer=http://localhost:1080/cgi-bin/nav.pl?page=menu&in=home", 
		"Snapshot=t3.inf", 
		"Mode=HTML", 
		LAST);
	
	lr_save_string(lr_paramarr_random("cities"),"depart");
	lr_output_message("%s",lr_eval_string("{depart}"));
	lr_save_string(lr_paramarr_random("cities"),"arrive");
	lr_output_message("%s",lr_eval_string("{arrive}"));
	
	while (strcmp(lr_eval_string("{arrive}"),lr_eval_string("{depart}")) == 0)	//меняем arrive, пока не получим arrive!=depart
	{
		lr_save_string(lr_paramarr_random("cities"),"arrive");
		lr_output_message("%s",lr_eval_string("{arrive}"));
	}  
    
	lr_end_transaction("open_Flights_page", LR_AUTO);
	
	lr_think_time(6);
	
	lr_start_transaction("choose_Flight_param");
	
	web_reg_save_param_regexp(
	"ParamName=outboundFlight",
	"RegExp=outboundFlight\" value\=\"(.+?)\"",
	"Group=1",	
	SEARCH_FILTERS,
	"Scope=BODY",
	LAST);
	
	web_reg_find(
	"Search=Body",
	"Text=<blockquote>Flight departing from <B>{depart}</B> to <B>{arrive}</B>",
	LAST);
	
	web_submit_data("reservations.pl", 
		"Action=http://localhost:1080/cgi-bin/reservations.pl", 
		"Method=POST", 
		"TargetFrame=", 
		"RecContentType=text/html", 
		"Referer=http://localhost:1080/cgi-bin/reservations.pl?page=welcome", 
		"Snapshot=t4.inf", 
		"Mode=HTML", 
		ITEMDATA, 
		"Name=advanceDiscount", "Value=0", ENDITEM, 
		"Name=depart", "Value={depart}", ENDITEM, 
		"Name=departDate", "Value={departDate}", ENDITEM, 
		"Name=arrive", "Value={arrive}", ENDITEM, 
		"Name=returnDate", "Value={returnDate}", ENDITEM, 
		"Name=numPassengers", "Value=1", ENDITEM, 
		"Name=seatPref", "Value={seatPref}", ENDITEM, 
		"Name=seatType", "Value={seatType}", ENDITEM, 
		"Name=.cgifields", "Value=roundtrip", ENDITEM, 
		"Name=.cgifields", "Value=seatType", ENDITEM, 
		"Name=.cgifields", "Value=seatPref", ENDITEM, 
		"Name=findFlights.x", "Value=24", ENDITEM, 
		"Name=findFlights.y", "Value=8", ENDITEM, 
		LAST);
	lr_end_transaction("choose_Flight_param", LR_AUTO);
	
	lr_start_transaction("choose_Flight");
	
	web_reg_find("Search=Body",
	"Text/IC=name\=\"firstName\" value\=\"{firstName}",
	LAST);
	
	web_reg_find("Search=Body",
	"Text/IC=name\=\"lastName\" value\=\"{lastName}",
	LAST);

	web_submit_data("reservations.pl_2", 
		"Action=http://localhost:1080/cgi-bin/reservations.pl", 
		"Method=POST", 
		"TargetFrame=", 
		"RecContentType=text/html", 
		"Referer=http://localhost:1080/cgi-bin/reservations.pl", 
		"Snapshot=t5.inf", 
		"Mode=HTML", 
		ITEMDATA, 
		"Name=outboundFlight", "Value={outboundFlight}", ENDITEM, 
		"Name=numPassengers", "Value=1", ENDITEM, 
		"Name=advanceDiscount", "Value=0", ENDITEM, 
		"Name=seatType", "Value={seatType}", ENDITEM, 
		"Name=seatPref", "Value={seatPref}", ENDITEM, 
		"Name=reserveFlights.x", "Value=38", ENDITEM, 
		"Name=reserveFlights.y", "Value=3", ENDITEM, 
		LAST);
	
	lr_end_transaction("choose_Flight", LR_AUTO);
	
	lr_think_time(4);

	lr_start_transaction("submit_data");
	
	web_reg_find("Search=Body",
	"Text/IC={firstName}{lastName}'s Flight Invoice",
	LAST);
	web_reg_find("Search=Body",
	"Text/IC=from {depart} to {arrive}",
	LAST);
		
	web_submit_data("reservations.pl_3", 
		"Action=http://localhost:1080/cgi-bin/reservations.pl", 
		"Method=POST", 
		"TargetFrame=", 
		"RecContentType=text/html", 
		"Referer=http://localhost:1080/cgi-bin/reservations.pl", 
		"Snapshot=t6.inf", 
		"Mode=HTML", 
		ITEMDATA, 
		"Name=firstName", "Value={firstName}", ENDITEM, 
		"Name=lastName", "Value={lastName}", ENDITEM, 
		"Name=address1", "Value={address1}", ENDITEM, 
		"Name=address2", "Value={address2}", ENDITEM, 
		"Name=pass1", "Value={firstName} {lastName}", ENDITEM, 
		"Name=creditCard", "Value={creditCard}", ENDITEM, 
		"Name=expDate", "Value={expDate}", ENDITEM, 
		"Name=oldCCOption", "Value=", ENDITEM, 
		"Name=numPassengers", "Value=1", ENDITEM, 
		"Name=seatType", "Value={seatType}", ENDITEM, 
		"Name=seatPref", "Value={seatPref}", ENDITEM, 
		"Name=outboundFlight", "Value={outboundFlight}", ENDITEM, 
		"Name=advanceDiscount", "Value=0", ENDITEM, 
		"Name=returnFlight", "Value=", ENDITEM, 
		"Name=JSFormSubmit", "Value=off", ENDITEM, 
		"Name=.cgifields", "Value=saveCC", ENDITEM, 
		"Name=buyFlights.x", "Value=63", ENDITEM, 
		"Name=buyFlights.y", "Value=9", ENDITEM, 
		LAST);
	
	lr_end_transaction("submit_data", LR_AUTO);
	
	lr_start_transaction("logout");

	web_reg_find("Search=Body",
		"Text=<title>Web Tours</title>",
		LAST);
	
	web_url("SignOff Button",
		"URL=http://localhost:1080/cgi-bin/welcome.pl?signOff=1",
		"TargetFrame=body",
		"Resource=0",
		"RecContentType=text/html",
		"Referer=http://localhost:1080/cgi-bin/nav.pl?page=menu&in=flights",
		"Snapshot=t28.inf",
		"Mode=HTML",
		LAST);
	lr_end_transaction("logout", LR_AUTO);
	
	lr_end_transaction("3_Task_Booking", LR_AUTO);

	return 0;
}