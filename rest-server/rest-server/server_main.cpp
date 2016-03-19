#include <iostream>
#include "MyListener.h"
#include <Windows.h>
#include <winsvc.h>

#define SERVICE_NAME TEXT("CppWindowsService")

SERVICE_STATUS serviceStatus;  // дл€ оповещени€ SCM текущего статуса сервиса
SERVICE_STATUS_HANDLE serviceStatusHandle;

void ServiceMain(int arc, char** argv);
int addLogMessage(char *a);
void ControlHandler(DWORD request);

int main(int argc, char* argv[])
{
	SERVICE_TABLE_ENTRY ServiceTable[1]; // структура, котора€ описывает точку входа дл€ сервис менеджера
	ServiceTable[0].lpServiceName = SERVICE_NAME;  // им€ сервиса
	ServiceTable[0].lpServiceProc = (LPSERVICE_MAIN_FUNCTION)ServiceMain;  //функци€-точка входа сервиса

	StartServiceCtrlDispatcher(ServiceTable);  // св€зывает сервис с SCM
}

// “очка входа сервиса
void ServiceMain(int arc, char** argv)
{
	int error;
	int i = 0;

	serviceStatus.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
	serviceStatus.dwCurrentState = SERVICE_START_PENDING;
	serviceStatus.dwControlsAccepted = SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_SHUTDOWN;
	serviceStatus.dwWin32ExitCode = 0;
	serviceStatus.dwServiceSpecificExitCode = 0;
	serviceStatus.dwCheckPoint = 0;
	serviceStatus.dwWaitHint = 0;

	serviceStatusHandle = RegisterServiceCtrlHandler(SERVICE_NAME, (LPHANDLER_FUNCTION)ControlHandler);  // регистрируем ф-цию котора€ будет обрабатывать управл€ющие запросы от SCM, например, запрос на остановку
	if (serviceStatusHandle == (SERVICE_STATUS_HANDLE)0)
		return;

	//error = InitService();
	//if (error)
	//{
	//	serviceStatus.dwCurrentState = SERVICE_STOPPED;
	//	serviceStatus.dwWin32ExitCode = -1;
	//	SetServiceStatus(serviceStatusHandle, &serviceStatus);  // изменение статуса сервиса
	//	return;
	//}

	serviceStatus.dwCurrentState = SERVICE_RUNNING;
	SetServiceStatus(serviceStatusHandle, &serviceStatus);

	while (serviceStatus.dwCurrentState == SERVICE_RUNNING)
	{
		char buffer[255];
		sprintf_s(buffer, "%u", i);  // вывод в массив buffer
		int result = addLogMessage(buffer);
		if (result)
		{
			serviceStatus.dwCurrentState = SERVICE_STOPPED;
			serviceStatus.dwWin32ExitCode = -1;
			SetServiceStatus(serviceStatusHandle, &serviceStatus);
			return;
		}
		i++;
	}

	return;
}

int addLogMessage(char *a)
{
	printf("%s", a);
	return 1;
}

// ‘ункци€ по обработке запросов. ¬ызываетс€ каждый раз, как SCM шлет запросы на изменени€ состо€ни€ сервиса. ¬ основном используетс€ дл€ описани€ корректной завершении работа сервиса
void ControlHandler(DWORD request)
{
	switch (request)
	{
	case SERVICE_CONTROL_STOP:
		addLogMessage("Stopped.");

		serviceStatus.dwWin32ExitCode = 0;
		serviceStatus.dwCurrentState = SERVICE_STOPPED;
		SetServiceStatus(serviceStatusHandle, &serviceStatus);
		return;

	case SERVICE_CONTROL_SHUTDOWN:
		addLogMessage("Shutdown");

		serviceStatus.dwWin32ExitCode = 0;
		serviceStatus.dwCurrentState = SERVICE_STOPPED;
		SetServiceStatus(serviceStatusHandle, &serviceStatus);
		return;

	default:
		break;
	}

	SetServiceStatus(serviceStatusHandle, &serviceStatus);

	return;
}

//int main()
//{
//	setlocale(LC_ALL, "Russian");
//	MyListener listener(L"http://localhost:8080/restdemo");  
//	listener.start();
//
//	getchar();
//	return 0;
//}