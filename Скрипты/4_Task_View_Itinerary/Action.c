Action()
{
	lr_start_transaction("4_Task_View_Itinerary");
	
	
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
	
	lr_start_transaction("open_Itinerary_list");
	
	// проверяем, пришла ли пустая ли страница с листами

	web_reg_save_param_regexp(						
	"ParamName=empty_list",
	"RegExp=(No flights have been reserved)",
	"NotFound=WARNING",
	"Ordinal=ALL",
	"Group=1",
	SEARCH_FILTERS,
	"Scope=BODY",
	LAST);
	
	////ищем номера рейсов, если есть
	
	web_reg_save_param_regexp(						
	"ParamName=list_flightID",
	"RegExp=name\=\"flightID\" value\=\"(.+?)\"",
	"NotFound=WARNING",
	"Ordinal=ALL",
	"Group=1",
	SEARCH_FILTERS,
	"Scope=BODY",
	LAST);
	
	web_url("Itinerary Button", 
		"URL=http://localhost:1080/cgi-bin/welcome.pl?page=itinerary", 
		"TargetFrame=body", 
		"Resource=0", 
		"RecContentType=text/html", 
		"Referer=http://localhost:1080/cgi-bin/nav.pl?page=menu&in=home", 
		"Snapshot=t3.inf", 
		"Mode=HTML", 
		LAST);
	
	if (strcmp(lr_eval_string("{empty_list_count}"),"0") == 0)	
		
	{//лист не пуст
			if (strcmp(lr_eval_string("{list_flightID_count}"),"0") == 0)
			{
				////лист не пуст, но не нашёл номеров рейсов = ошибка
				lr_error_message("list is not empty, but flightID not found");

			}
			else
			{
				lr_output_message ("Intineraries found");
				
			}
	}
	else 
	{//лист пуст
		lr_output_message ("Intineraries list empty");
	}

	lr_end_transaction("open_Itinerary_list", LR_AUTO);
	
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
	
	lr_end_transaction("4_Task_View_Itinerary", LR_AUTO);

	return 0;
}