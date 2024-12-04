int i;
Action()
{
	lr_start_transaction("5_Task_Delete_Booking");
	
	
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
	
	////записываем номера рейсов, если есть
	
	web_reg_save_param_regexp(						
	"ParamName=list_flightID",
	"RegExp=name\=\"flightID\" value\=\"(.+?)\"",
	"NotFound=WARNING",
	"Ordinal=ALL",
	"Group=1",
	SEARCH_FILTERS,
	"Scope=BODY",
	LAST);
	
	////записываем cgifields, если есть
	web_reg_save_param_regexp(						
	"ParamName=c_cgifields",
	"RegExp=name\=\".cgifields\" value\=\"(.+?)\"",
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
		
	{//маршрутный лист не пуст
			if (strcmp(lr_eval_string("{list_flightID_count}"),"0") == 0)
			{
				////лист не пуст, но не нашёл номеров рейсов = ошибка
				lr_error_message("list is not empty, but flightID not found");
			}
			else
			{
				///только теперь можно удалять
				
					lr_start_transaction("delete_random_Intinerary");

						lr_param_sprintf("c_buffer",
				  		"%s=on&",
				  		lr_paramarr_random("c_cgifields"));
							
						for (i=1;i<=atoi(lr_eval_string("{list_flightID_count}"));i++)
							 {
							 lr_param_sprintf("c_buffer", "%sflightID=%s&",
							   lr_eval_string("{c_buffer}"),
							   lr_paramarr_idx("list_flightID",
							   i));
							
							 lr_param_sprintf("c_buffer",
							   "%s.cgifields=%s&",
							   lr_eval_string("{c_buffer}"),
							   lr_paramarr_idx("c_cgifields", i));
							}
						
						lr_save_string(lr_eval_string("{c_buffer}removeFlights.x=36&removeFlights.y=4"),
				   "c_wcr");
						
							////записываем новые cgifields, если есть. нам нужен только new_c_cgifields_count
							web_reg_save_param_regexp(						
							"ParamName=new_c_cgifields",
							"RegExp=name\=\".cgifields\" value\=\"(.+?)\"",
							"NotFound=WARNING",
							"Ordinal=ALL",
							"Group=1",
							SEARCH_FILTERS,
							"Scope=BODY",
							LAST);
							
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
								
							web_custom_request("itinerary.pl",
							 "URL=http://localhost:1080/cgi-bin/itinerary.pl",
							 "Method=POST",
							 "Resource=0",
							 "RecContentType=text/html",
							 "Referer=http://localhost:1080/cgi-bin/itinerary.pl",
							 "Snapshot=t4.inf",
							 "Mode=HTTP",
							 "Body={c_wcr}",
							 LAST);
							
							if (strcmp(lr_eval_string("{list_flightID_count}"),"1") == 0) //если в прошлый раз была последняя запись

							{ 	//ожидаем, что перед нами страница "No flights have been reserved"
								if (strcmp(lr_eval_string("{empty_list_count}"),"0") == 0) 
								{
									lr_error_message("list is not empty, but list_flightID_count 0");
								}
								else
								{
									lr_output_message ("Delete sucsess, there are no Intineraries anymore");
								}
								
							}
							else
							{
								 //ожидаем, что записей станет на одну меньше после удаления
								if (atoi(lr_eval_string("{new_c_cgifields_count}"))+1 != atoi(lr_eval_string("{c_cgifields_count}")))
								{
									lr_error_message("list_flightID_count not 0, but new_c_cgifields_count-1 and c_cgifields_count do not matches");
								}
								else
								{
									lr_output_message( "Delete sucsess, there are %s Intineraries now", lr_eval_string("{new_c_cgifields_count}"));
								}

							}
							
					lr_end_transaction("delete_random_Intinerary", LR_AUTO);

			}
	}
	else 
	{//маршрутный лист пуст (нужно ли заменить на ошибку, т.к. скрипт ничего не удалил из-за отсутствия записей??)
		lr_output_message ("Intineraries list empty, no data to delete");
		//lr_error_message("Intineraries list empty, no data to delete");
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
	
	lr_end_transaction("5_Task_Delete_Booking", LR_AUTO);

	return 0;
}