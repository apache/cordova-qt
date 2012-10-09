/*
 *
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 *
*/

var pluginObjects = {}


function addPlugin(pluginName, pluginObject) {
    pluginObjects[pluginName] = pluginObject
}

function messageHandler(message) {
    var received = eval('('+message.data+')')
    if (typeof received == 'undefined')
        return false
    if (typeof received.messageType == 'undefined')
        return false
    if (received.messageType == "callPluginFunction") {
        if (typeof received.plugin == 'undefined' ||
                typeof received.func == 'undefined')
            return false
        execMethod(received.plugin, received.func, received.params)
    }
    return true
}


function execMethod(pluginName, functionName, params) {
    if( typeof pluginObjects[pluginName][functionName] != "function" )
        return false
    pluginObjects[pluginName][functionName].apply(this, params)
    return true
}


function execMethodOld(pluginName, functionName, params) {
    if( typeof pluginObjects[pluginName][functionName] != "function" )
        return false

    var paramsString = JSON.stringify(params)
    if (paramsString.charAt(0) == "[")
        paramsString = paramsString.substring(1, paramsString.length-1)

    eval("pluginObjects[pluginName][functionName]("+paramsString+")")
    return true
}
