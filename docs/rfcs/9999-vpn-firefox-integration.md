- Status: Draft
- Date: 2023-05-29
- Authors:
    - [@strseb](https://github.com/strseb)
    - [@lesleyjanenorton](https://github.com/lesleyjanenorton)
    - [@oskirby](https://github.com/oskirby)
    - [Santiago]()
- RFC PR: -
- Implementation GitHub issue: -

-----

# Problem Statement


| ![](data:image/jpeg;base64,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) | VPNs currently allow you to select the country through which you want your traffic to emerge to the Internet and users use this to express a variety of geolocation preferences. These controls are not flexible enough - users typically can select a single location through which all traffic should go through. |
|---|---|

The Current plan is to write a new Web-Extension that extends on top of the API the VPN-Client offers to Multi Account Container. 

The vision is, for this integration giving users the options to:
 - Enable The VPN only for Firefox 
 - Disable The VPN only for Firefox 
 - Exclude a Page from VPN protection
 - Specify an Exit Location a Page.

We already have integrated into Firefox with [Multi Account Containers](https://github.com/mozilla/multi-account-containers) which allows users to specify an exit location for a container. 
The following parts expect knowledge on how the MAC Extension works, for a refresher scroll to appendix :) 

----

## Preconditions

For this feature to be enabled, a Firefox user on the desktop needs to have the VPN installed, be logged in and have an active subscription. 


## VPN API 


### Networking
Using the [proxy api](https://developer.mozilla.org/en-US/docs/Mozilla/Add-ons/WebExtensions/API/proxy) a network extension can decide where each network request should be proxied to. So from a network request perspective, we only have 4 possible routes to take. 


| x               | Device VPN is On | Device VPN is OFF |
|-----------------|------------------|-------------------|
| Page VPN is On  | 1                | 2                 |
| Page VPN is Off | 3                | 4                 |

In the following sections, we will explain what changes we need inside the client to support each of the cases and where the extension must proxy traffic to. 


#### State 1: Device VPN is On & Page VPN is On
In this State the Extension can simply set the Proxy for those network requests to `null` to use the user's active server, or if a custom exit location is set, to the `socks5` proxy of that endpoint. No changes are needed, this is how MAC works. 


#### State 2: Device VPN is OFF & Page VPN is On
Currently, the VPN has the following states:
```cpp
  enum State {
    StateOff, // Noting protected
    StateOn, // Whole device protected
    (...) // unrelated intermediate states
  };
```
An extension can send `{"t":"activate"}` to the VPN to turn on whole-device protection. 

We would introduce a new state `StateOnPartial` that will be activated on a call of an extension to `{"t":"activate"}` instead. It is similar to `StateOn` with the following properties: 
- The `allowedIp` for this activation is set to `10.124.0.0/20` so that only the Mullvad Exit Proxies are accessible on the VPN Network Device. Therefore, all other traffic will not use the VPN. 
- The DNS server is set to any Mullvad Server, so that the mullvad proxies are resolvable. 
- The Client UI does not show an activation. (or a new special UI state)
> Q: Do we need a new UI state ? or like a "firefox is using the vpn?" 
- If the Client activates, it will attempt to switch to `StateOn` and protect the whole device.
- An Extension may request `{"t":"deactivate"}` only if the client is in `StateOnPartial` and it was the activator. The Client will then go to `StateOff`.

The extension must then set the proxy to the `socks5` URL of the current `exitServer` OR, if a custom exit location is set, to the `socks5` proxy of that endpoint.

#### State 3: Device VPN is On & Page VPN is Off

If the full device is protected and the VPN integration is enabled, we will utilize a Socks5 Proxy running on localhost that will make sure its outgoing traffic utilizes the non-vpn default route. 

For that, we will create an external binary that 
- opens a socks5 proxy locally on a random port
- generates a username and password for this session
- prints the port, username, password to STDOUT so that the parent process may gather that info. 

The VPN client will then pass that info to the Extension inside it's `status` response as an extra field. The extension must use that proxy for pages wanting to bypass the VPN.

For Platforms where split tunneling is supported, the proxy is permanently fixed in the split-tunnel rule list, making sure the vpn connection cannot be used. 

For Platforms where split tunneling is not available, the proxy will use socket options to make sure the right network device is used.
> Q: do we need more details here? 

If the daemon has support for per-app firewall rules (currently Windows), we will make sure only Firefox may access the port of the proxy to make sure other local apps cannot use the proxy to get the real IP of the user. 



#### State 4: Device VPN is OFF & Page VPN is Off
This State is trivial, no proxy needs to be set by the extension. 

### Other API Changes 

#### Renaming
We will rename the current C++ Extension API classes to ... ExtensionAPI. They are currently under `server/` and called `ServerHandler` && `ServerConnection` which is an overloaded term for a VPN. 

#### Split-Tunnel Lists
Instead of reporting all excluded apps in `disabled_apps`, we will limit the response to executables containing `firefox`. The Extension may then limit it's funcitonality until resolved, as Firefox cannot access the VPN if it is excluded.  



## Appendix 


### Refresher MAC+VPN
MAC allows users to associate websites with containers. The VPN integration allows selecting a location from the VPN server list as the exit location for each container. It Requires Mozilla VPN to be installed and active to work. 


For the MAC + VPN we have three actors: 
- MAC - [Multi Account Containers](https://github.com/mozilla/multi-account-containers) the Plugin opens a [Native Messaging port](https://developer.mozilla.org/en-US/docs/Mozilla/Add-ons/WebExtensions/Native_messaging) using `browser.runtime.connectNative("mozillavpn");`
- NativeMessaging.rs (Bridge) - Is the Process sitting on the other side of ""? "STDIN" and "STDOUT" are connected to the Extension. After validating that the Port is related to an Extension that may access the client, it will establish a tcp connection and act as a transparent message bridge.
- Client: The VPN Client. On Desktop, it creates a [TCP Server](https://github.com/mozilla-mobile/mozilla-vpn-client/blob/9e74f31118ce23b92a0d1d14b8e928f3cd7114ab/src/server/serverhandler.cpp#L25)

```mermaid 
sequenceDiagram
    participant MAC
    participant NM as Bridge
    participant VPN
    

    MAC->>NM: connectNative(extensionID)
    NM->>NM: checkIsValid(extensionID)
    NM->>VPN: connect TCP
    destroy NM
    NM->>MAC: status: vpn-client-up
    Note right of NM: Bridge is now a transparent relay
    MAC->>VPN: get-status
    VPN->>MAC: status: connected=false
    MAC->>VPN: activate
    VPN->>MAC: status: connected=true
    MAC->>VPN: get-servers
    destroy VPN
    VPN->>MAC: serverlist[ "mullad-1", "mullvad-2"]
    create participant Firefox
    MAC->>Firefox: Set Proxy for Container 001 to "socks5://mullvad-1"

```

### Current Message format 
```ts
// Over the Wire we send a Raw Buffer
type NMRequest = Buffer;
// The first byte is the Body Length
type NMRequestLength = NMRequest[0];
// The Body contains a UTF-8 encoded JSON blob
// That must parse into <ExtensionMessage>
type NMRequestBody = NMRequest[1:NMRequestLength];


interface ExtensionMessage{
    // Every request must have a type.
    // The response obj with have the type of the request or "invalidRequest"
    t:ExtensionMessageType 
}

type ExtensionMessageType = "activate" | "servers" | "disabled_apps" | "status" |"invalidRequest"
```




