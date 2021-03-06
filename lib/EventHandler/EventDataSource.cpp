#include "EventDataSource.h"


bool EventDataSource::SetEnabled(bool enabled) { 
    _isEnabled = enabled;
    return IsEnabled();
}

bool EventDataSource::IsAvailable() { 
    return (_isAvailable); 
}

bool EventDataSource::IsEnabled() { 
    return (_isAvailable && _isEnabled); 
}

bool EventDataSource::IsReady() { 
    return (_isAvailable && _isEnabled && (millis() > _nextReportMs)); 
}

void EventDataSource::Config(EventData *data, MyDebugger *dbg, byte devId) {
    _data = data;
    _dbg = dbg;
    _DevId = devId;
}

void EventDataSource::SetNextReportTime() { 
    _nextReportMs = millis() + _reportInterval; 
}

void EventDataSource::PostHandler(bool eventMatched, bool isRelated, bool pending) {
    if (!IsReady()) return;
    if (_dbg->require(210)) _dbg->log(210,0,"EventDataSource[%d]::PostHandler(%d,%d,%d)",_Device, eventMatched, isRelated, pending);
    if (_thisDataReady) {
        if (eventMatched && !isRelated){
            // handled other events, need to execute again immediately 
            _nextReportMs = 0;
        } else  {
            // either handled or no action for it, can delay longer
            _nextReportMs = millis() + _delayCheckMs;
        }
    } else if (pending) {
        _nextReportMs = millis() + _pendingCheckMs;
    } else {
        _nextReportMs = millis() + _continueCheckMs;
    }
}
