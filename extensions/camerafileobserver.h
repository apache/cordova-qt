/*
 ============================================================================
 Name		 : CCameraFileObserver.h
 Author		 : chenziteng@163.com
 Version	 : 1.0
 Copyright   : Your copyright notice
 Description : CCameraFileObserver declaration
 ============================================================================
 */

#ifndef CCAMERAFILEOBSERVER_H
#define CCAMERAFILEOBSERVER_H

#include <e32base.h>	// For CActive, link against: euser.lib
#include <e32std.h>		// For RTimer, link against: euser.lib
#include <e32property.h> // RProperty

class MCameraFileObserver 
	{
public:
	virtual void NewCameraFileL(const TFileName& aFileName) = 0;
	};

class CCameraFileObserver : public CActive
	{
public:
	// Cancel and destroy
	~CCameraFileObserver();

	// Two-phased constructor.
	static CCameraFileObserver* NewL(MCameraFileObserver& aObserver);

	// Two-phased constructor.
	static CCameraFileObserver* NewLC(MCameraFileObserver& aObserver);

private:
	// C++ constructor
	CCameraFileObserver(MCameraFileObserver& aObserver);

	// Second-phase constructor
	void ConstructL();

private:
	// From CActive
	// Handle completion
	void RunL();

	// How to cancel me
	void DoCancel();

public:
	// New functions
	// Function for making the initial request
	void Start();

	void GetLastFileL(TFileName& aFileName);

private:
	MCameraFileObserver& iObserver;
	RProperty iProperty;
	};

#endif // CCAMERAFILEOBSERVER_H
