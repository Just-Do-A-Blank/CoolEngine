#pragma once

/// <summary>
/// Tthe list of possible event types
/// </summary>
enum class EventType 
{
	KeyPressed, KeyReleased,
	MouseButtonPressed, MouseButtonReleased, MouseMoved,
	TriggerEnter, TriggerHold, TriggerExit,
	CollisionEnter, CollisionHold, CollisionExit,
    CreateBullet, Pickup
};

/// <summary>
/// Base event for obeservers.
/// Particular event types maybe found in the folder such as Collision and KeyPress events
/// include them if required.
/// </summary>
class Event 
{
public:

    /// <summary>
    /// Constructor that sends event with data, manual deconstruction of the data required
    /// </summary>
	Event(EventType eventID, void* data);

    /// <summary>
    /// Alternate constructor to just call an event proc instead of with data
    /// </summary>
	Event(EventType eventID);

	virtual ~Event();

    /// <summary>
    /// The event ID which in this context is the broad type of the event for casting
    /// </summary>
	const EventType GetEventID();

    /// <summary>
    /// Get the data passed in when creating the event.
    /// Generally not required, casting is prefered.
    /// </summary>
	void* GetData();

protected:

    /// <summary>
    /// The event ID which in this context is the broad type of the event for casting
    /// </summary>
	EventType m_eventID;

	void* m_data;
};