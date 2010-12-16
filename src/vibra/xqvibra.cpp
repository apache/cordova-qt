#include "xqvibra.h"
#include "xqvibra_p.h"

/*!
    \class XQVibra

    \brief The XQVibra class is used to control the device's vibra. The XQVibra
    class also provides information about the vibra setting in the user profile.
*/

/*! \var XQVibra::InfiniteDuration
    With this value the vibra function can be set to run indefinitely (Note! This depends on the hardware.)
*/
/*! \var XQVibra::MaxIntensity
    Maximum intensity as a percentage
*/
/*! \var XQVibra::MinIntensity
    Minumum intensity as a percentage
*/

/*!
    Constructs a XQVibra object with the given parent.
    Call error() to get an XQVibra::Error value that indicates which error occurred during initialisation, if any.
    \sa start(), setIntensity(), error()
*/
XQVibra::XQVibra(QObject* parent)
    : QObject(parent), d(new XQVibraPrivate(this))
{
}

/*!
    Destroys the XQVibra object.
*/
XQVibra::~XQVibra()
{
    delete d;
}

/*!
    \enum XQVibra::Error

    This enum defines the possible errors for a XQVibra object.
*/
/*! \var XQVibra::Error XQVibra::NoError
    No error occured.
*/
/*! \var XQVibra::Error XQVibra::OutOfMemoryError
    Not enough memory.
*/
/*! \var XQVibra::Error XQVibra::ArgumentError
    Duration is invalid.
*/
/*! \var XQVibra::Error XQVibra::VibraInUseError
    Vibra is already in used by other client.
*/
/*! \var XQVibra::Error XQVibra::HardwareError
    There is a hardware error.
*/
/*! \var XQVibra::Error XQVibra::TimeOutError
    Timeout occurred while controlling the vibra function.
*/
/*! \var XQVibra::Error XQVibra::VibraLockedError
    The vibra function is locked and unavailable due to too much continuous use. It might also have been explicitly blocked by 
    some vibration sensitive accessory.
*/
/*! \var XQVibra::Error XQVibra::AccessDeniedError
    The vibra setting in the user profile is not set.
*/
/*! \var XQVibra::Error XQVibra::UnknownError
    Unknown error.
*/

/*!
    \enum XQVibra::Status

    This enum defines the possible statuses of the vibra function
*/
/*! \var XQVibra::Status XQVibra::StatusNotAllowed
    Vibra is set off in the user profile or status is unknown
*/
/*! \var XQVibra::Status XQVibra::StatusOff
    Vibra function is not currently activated
*/
/*! \var XQVibra::Status XQVibra::StatusOn
    Vibra function is activated
*/

/*!
    Switches on the vibra function. If duration hasn't been set the vibration
    continues indefinitely until it is stopped with the stop() function. If you
    call start() when the vibra function has already been activated the current
    vibra timer is stopped and the new timer starts immediately. Note that the
    hardware may restrict the maximum vibration time.

    \param duration Specifies how long the vibra function should be activated
    \return If false is returned, an error has occurred. Call error() to get the 
    XQVibra::Error value that indicates which error occurred
    \sa stop(), setIntensity(), error()
*/
bool XQVibra::start(int duration)
{
    return d->start(duration);
}

/*!
    Interrupts the device vibr function immediately.
    
    \return If false is returned, an error has occurred. Call error() to get the
    XQVibra::Error value that indicates which error occurred
    \sa start(), setIntensity(), error() 
*/
bool XQVibra::stop()
{
    return d->stop();
}

/*!
    Sets the intensity of the vibration. The allowed values for the intensity are
    between -100 and 100 percent. 0 means no vibrating.
    
    NOTE: The device might have hardware-imposed limits on the supported
    vibra intensity values, so actual effect might vary between devices.

    \param intensity Intensity of the vibra
    \return If false is returned, an error has occurred. Call error() to get the 
    XQVibra::Error value that indicates which error occurred
    \sa error()
*/
bool XQVibra::setIntensity(int intensity)
{
    return d->setIntensity(intensity);
}

/*!
    Returns the current status of the vibration. This function can also be used to check if the
    current profile allows the vibra function so that your application can act according to the
    profile settings, i.e. only enabling the vibra function if the current profile allows it.

    \return current status
    \sa statusChanged()
*/
XQVibra::Status XQVibra::currentStatus() const
{
    return d->currentStatus();
}

/*!
    Returns the type of error that occurred if the latest function call failed; otherwise returns NoError
    \return Error code
*/
XQVibra::Error XQVibra::error() const
{
    return d->error();
}

/*!
    \fn void XQVibra::statusChanged(Status status)

    This signal is emitted when there is a change in the vibration status.

    \param status a vibration status
    \sa currentStatus()
*/

// End of file
