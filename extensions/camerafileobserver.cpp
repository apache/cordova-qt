/*
 ============================================================================
 Name		 : CCameraFileObserver.cpp
 Author		 : chenziteng@163.com
 Version	 : 1.0
 Copyright   : Your copyright notice
 Description : CCameraFileObserver implementation
 ============================================================================
 */

#include "CameraFileObserver.h"

// UID and key copied from the S60 Camera application source code
const TUid KPSUidCamcorderNotifier = {0x101F882E};
const TUint32 KCamLatestFilePath = 0x00000001;

CCameraFileObserver::CCameraFileObserver(MCameraFileObserver& aObserver) :
	CActive(EPriorityStandard), iObserver(aObserver)
	{
	CActiveScheduler::Add(this); // Add to scheduler
	}

CCameraFileObserver* CCameraFileObserver::NewLC(MCameraFileObserver& aObserver)
	{
	CCameraFileObserver* self = new (ELeave) CCameraFileObserver(aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	return self;
	}

CCameraFileObserver* CCameraFileObserver::NewL(MCameraFileObserver& aObserver)
	{
	CCameraFileObserver* self = CCameraFileObserver::NewLC(aObserver);
	CleanupStack::Pop(); // self;
	return self;
	}

void CCameraFileObserver::ConstructL()
	{
	// WriteDeviceData if the UID is not the SID of the process
	TInt err = RProperty::Define(KPSUidCamcorderNotifier, KCamLatestFilePath, RProperty::ELargeText);
	if(!((err==KErrNone)||(err==KErrAlreadyExists)))
		{
		User::LeaveIfError(err);
		}
	err = iProperty.Attach(KPSUidCamcorderNotifier, KCamLatestFilePath);
	User::LeaveIfError(err);
	}

CCameraFileObserver::~CCameraFileObserver()
	{
	Cancel(); // Cancel any request, if outstanding
	iProperty.Close(); // Destroy the RTimer object
	// Delete instance variables if any
	}

void CCameraFileObserver::DoCancel()
	{
	iProperty.Cancel();
	}

void CCameraFileObserver::RunL()
	{
	TInt err = iStatus.Int();
	iProperty.Subscribe(iStatus);
	SetActive(); // Tell scheduler a request is active
	if(err==KErrNone)
		{
		TFileName fn;
		GetLastFileL(fn);
		iObserver.NewCameraFileL(fn);
		}
	}

void CCameraFileObserver::Start()
	{
	Cancel(); // Cancel any request, just to be sure
	iProperty.Subscribe(iStatus); // Set for later
	SetActive(); // Tell scheduler a request is active
	}

void CCameraFileObserver::GetLastFileL(TFileName& aFileName)
	{
	TInt err = RProperty::Get(KPSUidCamcorderNotifier, KCamLatestFilePath, aFileName);
	User::LeaveIfError(err);
	}
