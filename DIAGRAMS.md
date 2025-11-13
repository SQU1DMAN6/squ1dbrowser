# SQU1D Browser - System Diagrams

## System Architecture Diagram

```
┌──────────────────────────────────────────────────────────────────┐
│                         USER INTERACTION                          │
│                     (Keyboard, Mouse, Resize)                     │
└────────────────────────────┬─────────────────────────────────────┘
                             │
                             ▼
┌──────────────────────────────────────────────────────────────────┐
│                    BROWSER WINDOW (C++)                          │
│  ┌────────────────────────────────────────────────────────────┐  │
│  │                  SDL2 Event Loop                           │  │
│  │  ┌──────────────────────────────────────────────────────┐ │  │
│  │  │ Navigation Buttons, URL Bar, Tab Management         │ │  │
│  │  │ ← → ⟳ │ google.com │ + │ Tab1 │ Tab2 │ Tab3      │ │  │
│  │  └──────────────────────────────────────────────────────┘ │  │
│  │  ┌──────────────────────────────────────────────────────┐ │  │
│  │  │         Rendered Content Display Area               │ │  │
│  │  │      (Shows pixel buffer from renderer)             │ │  │
│  │  └──────────────────────────────────────────────────────┘ │  │
│  └────────────────────┬──────────────────────────────────────┘  │
│                       │                                          │
└───────────────────────┼──────────────────────────────────────────┘
                        │
                        │ IPC Messages
                        │ (render requests)
                        ▼
┌──────────────────────────────────────────────────────────────────┐
│              RENDERER BRIDGE (C++ ↔ Rust)                        │
│  ┌────────────────────────────────────────────────────────────┐  │
│  │  Serialize C++ requests → Spawn Rust process             │  │
│  │  Receive pixel data ← Deserialize Rust response          │  │
│  └────────────────────────────────────────────────────────────┘  │
└────────────────────────┬─────────────────────────────────────────┘
                         │
                         │ IPC Communication
                         │ (pixel buffers)
                         ▼
┌──────────────────────────────────────────────────────────────────┐
│               RENDERER ENGINE (Rust)                             │
│  ┌────────────────────────────────────────────────────────────┐  │
│  │                   REQUEST HANDLER                          │  │
│  │  Receives: URL, Width, Height                            │  │
│  └────────────────────┬─────────────────────────────────────┘  │
│                       │                                          │
│  ┌────────────────────▼─────────────────────────────────────┐  │
│  │               HTTP CLIENT                                │  │
│  │  Fetches HTML content from network                      │  │
│  └────────────────────┬─────────────────────────────────────┘  │
│                       │                                          │
│  ┌────────────────────▼─────────────────────────────────────┐  │
│  │               HTML PARSER                                │  │
│  │  Tokenizes HTML → Builds DOM Tree                       │  │
│  └────────────────────┬─────────────────────────────────────┘  │
│                       │                                          │
│  ┌────────────────────▼─────────────────────────────────────┐  │
│  │               CSS PARSER                                 │  │
│  │  Extracts styles → Applies to DOM nodes                 │  │
│  └────────────────────┬─────────────────────────────────────┘  │
│                       │                                          │
│  ┌────────────────────▼─────────────────────────────────────┐  │
│  │               LAYOUT ENGINE                              │  │
│  │  Calculates positions and sizes (Layout Tree)            │  │
│  └────────────────────┬─────────────────────────────────────┘  │
│                       │                                          │
│  ┌────────────────────▼─────────────────────────────────────┐  │
│  │               RENDERER                                   │  │
│  │  Paints layout tree to RGBA pixel buffer                 │  │
│  └────────────────────┬─────────────────────────────────────┘  │
│                       │                                          │
└───────────────────────┼──────────────────────────────────────────┘
                        │
                        │ Response: RGBA Buffer
                        │ (width × height × 4 bytes)
                        ▼
                   [Back to Bridge]
```

## Data Flow Diagram

```
┌─────────────┐
│  User Types │
│   URL       │
└──────┬──────┘
       │
       ▼
┌──────────────────┐       ┌────────────────┐
│  URL Validation  │───────│  URL History   │
└──────┬───────────┘       └────────────────┘
       │
       ▼
┌──────────────────┐
│ HTTP Request     │
│ (Async, Hyper)   │
└──────┬───────────┘
       │
       ▼ HTML Response
┌──────────────────┐
│ HTML Parser      │
│ Tokenization     │
└──────┬───────────┘
       │
       ▼ DOM Tree
┌──────────────────┐      ┌──────────────────┐
│ CSS Rules        │◄─────│ CSS Parser       │
│ Collection       │      │ (from <style>)   │
└──────┬───────────┘      └──────────────────┘
       │
       ▼ Styled DOM
┌──────────────────┐
│ Layout Engine    │
│ Calculate:       │
│ - Position (x,y) │
│ - Size (w,h)     │
└──────┬───────────┘
       │
       ▼ Layout Tree
┌──────────────────┐
│ Renderer         │
│ Paint to:        │
│ RGBA Buffer      │
└──────┬───────────┘
       │
       ▼
┌──────────────────┐
│ Display on       │
│ Screen           │
└──────────────────┘
```

## Rendering Pipeline

```
Input: URL
┌─────────────────────────────────────┐
│ STAGE 1: FETCH & PARSE              │
├─────────────────────────────────────┤
│ • HTTP GET request to URL           │
│ • Receive HTML bytes                │
│ • Tokenize HTML                     │
│ • Build DOM tree                    │
│                                     │
│ Output: Parsed DOM with elements    │
│         and text nodes              │
└────────────────┬────────────────────┘
                 │
                 ▼
┌─────────────────────────────────────┐
│ STAGE 2: STYLE & APPLY RULES        │
├─────────────────────────────────────┤
│ • Extract CSS rules                 │
│ • Calculate specificity             │
│ • Apply matching rules to elements  │
│ • Create styled DOM                 │
│                                     │
│ Output: DOM with computed styles    │
└────────────────┬────────────────────┘
                 │
                 ▼
┌─────────────────────────────────────┐
│ STAGE 3: LAYOUT                     │
├─────────────────────────────────────┤
│ • Calculate element positions       │
│ • Calculate element sizes           │
│ • Create layout tree                │
│ • Store bounding boxes              │
│                                     │
│ Output: Layout tree with positions  │
└────────────────┬────────────────────┘
                 │
                 ▼
┌─────────────────────────────────────┐
│ STAGE 4: PAINT & RASTERIZE          │
├─────────────────────────────────────┤
│ • Traverse layout tree              │
│ • Fill background colors            │
│ • Draw element borders              │
│ • Render text content               │
│ • Write to pixel buffer             │
│                                     │
│ Output: RGBA pixel buffer           │
│         (width × height × 4)        │
└────────────────┬────────────────────┘
                 │
                 ▼
        [Display on screen]
```

## Tab Management Flow

```
┌─────────────────────────────────┐
│    Browser Window Started       │
│  (with 1 default tab)           │
└────────────┬────────────────────┘
             │
             ▼
    ┌────────────────────┐
    │  ACTIVE TABS LIST  │
    │  [Tab 1] ◄─ Active │
    │  [Tab 2]           │
    │  [Tab 3]           │
    └────────────────────┘
             │
    ┌────────┴────────────────┐
    │                         │
    ▼                         ▼
┌─────────────┐        ┌──────────────┐
│ User clicks │        │ User clicks  │
│ "+" button  │        │ on Tab 2     │
└────────┬────┘        └────────┬─────┘
         │                      │
         ▼                      ▼
    ┌────────────────────┐ ┌────────────────────┐
    │ Create new Tab 4   │ │ Mark Tab 2 active  │
    │ Mark Tab 4 active  │ │ Mark Tab 1 inactive│
    │ Mark Tab 1 inactive│ │ Request render     │
    │ Request render     │ │                    │
    └────────────────────┘ └────────────────────┘
             │                      │
             ▼                      ▼
    ┌────────────────────┐    ┌────────────────────┐
    │  ACTIVE TABS LIST  │    │  ACTIVE TABS LIST  │
    │  [Tab 1]           │    │  [Tab 1]           │
    │  [Tab 2]           │    │  [Tab 2] ◄─ Active │
    │  [Tab 3]           │    │  [Tab 3]           │
    │  [Tab 4] ◄─ Active │    │  [Tab 4]           │
    └────────────────────┘    └────────────────────┘
```

## UI Component Layout

```
┌─────────────────────────────────────────────────────┐
│ SQU1D Browser Window (1200x800)                     │
├─────────────────────────────────────────────────────┤
│ ┌────────────────────────────────────────────────┐  │
│ │ TOOLBAR (Height: 50px)                         │  │
│ │ ┌──┐ ┌──┐ ┌──┐ ┌──────────────────────┐ ┌──┐ │  │
│ │ │←│ │→│ │⟳│ │  google.com          │ │+│ │  │
│ │ └──┘ └──┘ └──┘ └──────────────────────┘ └──┘ │  │
│ │  10  50  90  130                   1100  1150   │  │
│ └────────────────────────────────────────────────┘  │
│ ┌────────────────────────────────────────────────┐  │
│ │ TAB BAR (Height: 30px)                         │  │
│ │ ┌──────────┐ ┌──────────┐ ┌──────────┐       │  │
│ │ │ Search   │ │ News     │ │ Images   │ ...   │  │
│ │ └──────────┘ └──────────┘ └──────────┘       │  │
│ │   10          115          220               │  │
│ └────────────────────────────────────────────────┘  │
│ ┌────────────────────────────────────────────────┐  │
│ │ CONTENT AREA (Height: 720px)                  │  │
│ │                                                │  │
│ │  Rendered web content from renderer           │  │
│ │  (HTML parsed and laid out)                   │  │
│ │                                                │  │
│ │                                                │  │
│ │                                                │  │
│ └────────────────────────────────────────────────┘  │
└─────────────────────────────────────────────────────┘

Coordinates:
- Window: (0, 0) to (1200, 800)
- Toolbar: (0, 0) to (1200, 50)
- Tab Bar: (0, 50) to (1200, 80)
- Content: (0, 80) to (1200, 800)
```

## Class Hierarchy

```
┌──────────────────────────────────────────────────────┐
│ Application Layer                                    │
├──────────────────────────────────────────────────────┤
│
│  main()
│  └─ BrowserWindow
│     ├─ TabManager
│     │  ├─ Tab[0]
│     │  ├─ Tab[1]
│     │  └─ Tab[n]
│     │
│     ├─ UIRenderer
│     │  └─ FrameBuffer (RGBA)
│     │
│     └─ RendererBridge
│        └─ [IPC to Rust Renderer]
│
└──────────────────────────────────────────────────────┘

┌──────────────────────────────────────────────────────┐
│ Renderer Layer (Rust)                                │
├──────────────────────────────────────────────────────┤
│
│  Document
│  ├─ root: Element
│  └─ styles: Vec<StyleRule>
│
│  Element
│  ├─ tag: String
│  ├─ attributes: HashMap<String, String>
│  ├─ children: Vec<Node>
│  └─ text_content: Option<String>
│
│  LayoutBox
│  ├─ element: Option<Element>
│  ├─ x, y, width, height: f32
│  └─ children: Vec<LayoutBox>
│
└──────────────────────────────────────────────────────┘
```

## State Transitions

```
┌──────────────────┐
│   App Started    │
│  (1 Tab, Idle)   │
└────────┬─────────┘
         │
    ┌────┴─────────────────────┬──────────────────┐
    │                          │                  │
    ▼                          ▼                  ▼
┌─────────────┐     ┌──────────────────┐   ┌────────────┐
│ User Types  │     │ User Clicks Tab  │   │ User Clicks│
│ URL & Enter │     │ or Back/Forward  │   │ "+ Button" │
└──────┬──────┘     └────────┬─────────┘   └──────┬─────┘
       │                     │                    │
       ▼                     ▼                    ▼
   ┌────────────────────────────────────────────────┐
   │     RENDERING STATE                            │
   │  - Fetching content                            │
   │  - Parsing HTML                                │
   │  - Calculating layout                          │
   │  - Rendering pixels                            │
   └──────────────┬─────────────────────────────────┘
                  │
                  ▼
   ┌────────────────────────────────────────────────┐
   │     IDLE STATE                                 │
   │  - Page displayed                              │
   │  - Waiting for user input                      │
   │  - Tab shows content                           │
   └────────────────────────────────────────────────┘
```

---

These diagrams provide visual understanding of:
1. System architecture and components
2. Data flow through the pipeline
3. Rendering stages
4. Tab management
5. UI layout
6. Class structure
7. State transitions

Use these to understand how different parts communicate and work together!
