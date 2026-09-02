import AudioToolbox
import Foundation

var description = AudioComponentDescription(
    componentType: kAudioUnitType_Effect,
    componentSubType: 0,
    componentManufacturer: 0,
    componentFlags: 0,
    componentFlagsMask: 0
)
var component: AudioComponent?
var discovered = 0
var loadable = 0

while true {
    component = AudioComponentFindNext(component, &description)
    guard let component else { break }
    discovered += 1
    var name: Unmanaged<CFString>?
    AudioComponentCopyName(component, &name)
    let displayName = (name?.takeRetainedValue() as String?) ?? "Unnamed Audio Unit"
    var instance: AudioComponentInstance?
    let result = AudioComponentInstanceNew(component, &instance)
    if result == noErr {
        loadable += 1
        if let instance { AudioComponentInstanceDispose(instance) }
        print("OK\t\(displayName)")
    } else {
        print("FAIL\t\(displayName)\tOSStatus \(result)")
    }
}

print("SUMMARY\t\(loadable)/\(discovered) effect Audio Units instantiated")