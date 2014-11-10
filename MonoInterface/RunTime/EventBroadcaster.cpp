#include "stdafx.h"
#include "EventBroadcaster.h"

void EventBroadcaster::RemoveListener(IMonoSystemListener *listener)
{
	// Delete the listener from the main list.
	int index = -1;
	this->listeners.ThroughEach
	(
		[&index, &listener](IMonoSystemListener *l, int i) { if (l == listener) index = i; }
	);
	if (index != -1)
	{
		this->listeners.RemoveAt(index);
	}
	// Remove listener from stage map.
	this->stageMap.ForEach
	(
		[&listener](int stageIndex, List<IMonoSystemListener *> *subscribers)
		{
			for (int i = 0; i < subscribers->Length; i++)
			{
				if (subscribers->At(i) == listener)
				{
					subscribers->RemoveAt(i);
					break;		// No need to proceed as there can only be one unique listener per stage.
				}
			}
		}
	);
}

void EventBroadcaster::SetInterface(IMonoInterface *inter)
{
	for (int i = 0; i < this->listeners.Length; i++)
	{
		this->listeners[i]->SetInterface(inter);
	}
}

//! Broadcasts PreInitialization event.
void EventBroadcaster::OnPreInitialization()
{
	for (int i = 0; i < this->listeners.Length; i++)
	{
		this->listeners[i]->OnPreInitialization();
	}
}
//! Broadcasts RunTimeInitializing event.
void EventBroadcaster::OnRunTimeInitializing()
{
	for (int i = 0; i < this->listeners.Length; i++)
	{
		this->listeners[i]->OnRunTimeInitializing();
	}
}
//! Broadcasts RunTimeInitialized event.
void EventBroadcaster::OnRunTimeInitialized()
{
	for (int i = 0; i < this->listeners.Length; i++)
	{
		this->listeners[i]->OnRunTimeInitialized();
	}
}
//! Broadcasts CryamblyInitilizing event.
void EventBroadcaster::OnCryamblyInitilizing()
{
	for (int i = 0; i < this->listeners.Length; i++)
	{
		this->listeners[i]->OnCryamblyInitilizing();
	}
}
//! Broadcasts CompilationStarting event.
void EventBroadcaster::OnCompilationStarting()
{
	for (int i = 0; i < this->listeners.Length; i++)
	{
		this->listeners[i]->OnCompilationStarting();
	}
}
//! Broadcasts CompilationComplete event.
void EventBroadcaster::OnCompilationComplete(bool success)
{
	for (int i = 0; i < this->listeners.Length; i++)
	{
		this->listeners[i]->OnCompilationComplete(success);
	}
}
//! Gathers initialization stages data for sending it to managed code.
int *EventBroadcaster::GetSubscribedStagesInfo(int &stageCount)
{
	// Gather information about all stages.
	for (int i = 0; i < this->listeners.Length; i++)
	{
		// Get stages.
		List<int> *stages = this->listeners[i]->GetSubscribedStages();
		if (stages)
		{
			// Put the listeners into the map.
			for (int j = 0; j < stages->Length; j++)
			{
				int currentStageIndex = stages->At(j);
				if (!this->stageMap.Contains(currentStageIndex))
				{
					this->stageMap.Add(currentStageIndex, new List<IMonoSystemListener *>(10));
				}
				this->stageMap.At(currentStageIndex)->Add(this->listeners[i]);
			}
			delete stages;
		}
	}
	// Get the stage indices.
	stageCount = this->stageMap.Length;
	int *stageIndices = new int[stageCount];
	int i = 0;
	this->stageMap.ForEach([stageIndices, &i](int stageIndex, List<IMonoSystemListener *> *subscribers)
	{
		stageIndices[i++] = stageIndex;
	});
	return stageIndices;
}
//! Broadcasts InitializationStage event.
//!
//! @param stageIndex Index of initialization stage that is happening.
void EventBroadcaster::OnInitializationStage(int stageIndex)
{
	auto stageList = this->stageMap.At(stageIndex);
	if (!stageList)
	{
		return;
	}

	for (int i = 0; i < stageList->Length; i++)
	{
		stageList->At(i)->OnInitializationStage(stageIndex);
	}
}
//! Broadcasts CryamblyInitilized event.
void EventBroadcaster::OnCryamblyInitilized()
{
	for (int i = 0; i < this->listeners.Length; i++)
	{
		this->listeners[i]->OnCryamblyInitilized();
	}
}
//! Broadcasts PostInitialization event.
void EventBroadcaster::OnPostInitialization()
{
	for (int i = 0; i < this->listeners.Length; i++)
	{
		this->listeners[i]->OnPostInitialization();
	}
}
//! Broadcasts Update event.
void EventBroadcaster::Update()
{
	for (int i = 0; i < this->listeners.Length; i++)
	{
		this->listeners[i]->Update();
	}
}
//! Broadcasts PostUpdate event.
void EventBroadcaster::PostUpdate()
{
	for (int i = 0; i < this->listeners.Length; i++)
	{
		this->listeners[i]->PostUpdate();
	}
}
//! Broadcasts Shutdown event.
void EventBroadcaster::Shutdown()
{
	for (int i = 0; i < this->listeners.Length; i++)
	{
		this->listeners[i]->Shutdown();
	}
}