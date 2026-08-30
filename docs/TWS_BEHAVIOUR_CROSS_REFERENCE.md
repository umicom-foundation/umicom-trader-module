<!--
Umicom Trader
File: docs/TWS_BEHAVIOUR_CROSS_REFERENCE.md

PURPOSE:
Turn public TWS product-behaviour research into an honest implementation map
for Umicom Trader and reusable Umicom Framework components.

AUTHOR AND ORGANISATION:
Sammy Hegab
Umicom Foundation

LICENCE:
MIT
-->

# Trading Workstation Behaviour Cross-Reference

## Why this document exists

Trader Workstation is a useful example of a dense desktop application that
still lets each user organise the screen for a particular job. Umicom Trader
should provide comparable classes of capability while keeping its own design,
terminology, code and safety model.

The research source is the official
[TWS user guide](https://www.ibkrguides.com/traderworkstation/getting-started.htm).
More focused references are linked beside the relevant behaviours below. No
TWS source code, branding, account connectivity or proprietary design asset is
part of Umicom.

The status words in this document mean:

- **Available**: a usable Framework or Trader implementation exists.
- **Foundation**: the reusable contract or view model exists, but some native
  interaction or live-provider work remains.
- **Planned**: design work is recorded, but the user should not be told that the
  feature is complete.

## Workspace and window behaviour

| Public TWS behaviour | Umicom status | Umicom implementation |
|---|---|---|
| A control centre exposes New Window, notifications, data state and a layout lock | **Foundation** | The Application Suite GTK workstation now supplies New Window and explicit Edit/Save/Cancel controls. Product notification and connection badges remain separate Framework services. |
| A searchable New Window menu opens tools | **Available** | `UmiUiWindowCatalogue` supports search, categories, recent-use ordering, singleton detection and open counts. Trader receives the control through its Framework workstation. |
| A layout must be unlocked before panels can be added, removed or rearranged | **Available** | `UmiUiWorkspaceCustomisation` owns guarded begin, commit and cancel edit sessions. Normal mode is locked. |
| Custom layouts have names and appear as tabs | **Foundation** | Framework supports add, clone, rename, activate and remove. Existing application layouts are selectable; creating and renaming tabs needs its final native dialog. |
| A blank layout can be populated and sized | **Foundation** | Catalogue-driven creation and normalised placement exist. Pointer docking previews and free-form resize handles remain planned. |
| Predefined layouts can be installed from a library | **Foundation** | Framework has a layout library and canonical Trader layouts. The final visual library should group layouts by job, explain them in plain language and preview their panels. |
| Locked tiles resist accidental close and reposition | **Available** | Framework mutation functions reject layout changes while locked. |
| Cancelling an edit returns to the previous arrangement | **Available** | The exact pre-edit layout is kept as a baseline and restored on cancel. |
| Panels can be snapped or grouped as a workspace | **Foundation** | Canonical left, centre, right, top, bottom and floating regions exist. Fine-grained snap targets and drag previews remain planned. |
| Pages and tools can move to other monitors | **Foundation** | Framework has monitor topology and persistent layout services. The lightweight Application Suite GTK control still needs a bridge to native detached windows and monitor recovery. |
| Selecting one detached window can bring the group forward | **Planned** | Add this as an opt-in desktop window-group policy, not an unconditional focus-stealing behaviour. |
| Settings can be saved, restored and recovered from an earlier day | **Foundation** | Framework already contains persistence, history and recovery services. Current lightweight edits must be committed through that semantic service before restart recovery is complete. |
| Global display configuration controls fonts, density, colour and table behaviour | **Foundation** | Theme, density and appearance contracts exist. Trader needs one clear settings surface and per-table column profiles. |

Relevant official behaviour references:

- [Mosaic Anchor Window and lock behaviour](https://www.ibkrguides.com/traderworkstation/mosaic-anchor-window.htm)
- [Custom layouts](https://www.ibkrguides.com/traderworkstation/custom-layouts.htm)
- [Predefined layouts](https://www.ibkrguides.com/traderworkstation/predefined-layouts.htm)
- [Mosaic File menu](https://www.ibkrguides.com/traderworkstation/mosaic-file-menu.htm)
- [Display configuration](https://www.ibkrguides.com/traderworkstation/display-configuration.htm)
- [Restore and save page settings](https://www.ibkrguides.com/traderworkstation/restore-and-save-page-settings.htm)

## Linked context groups

The official
[colour grouping guide](https://www.ibkrguides.com/traderworkstation/color-grouping-linked-panels.htm)
describes source, destination and source/destination windows. Umicom models the
same general interaction pattern as an original, product-neutral Framework
contract:

- a Watchlist may publish an Instrument context;
- Chart, Market Depth, News and Order Entry may receive that Instrument;
- a group has a visible colour token and one context kind;
- a window has only one active context-group assignment;
- source-only and destination-only roles prevent feedback loops;
- `umi_ui_window_group_assign` moves a window safely between groups;
- `umi_ui_window_group_route` resolves the receiving windows.

The next native UI step is a small chain-link control in every reusable panel
header. It must show the colour, role and shared subject in a tooltip. “No
group” must always be available.

## Trading tool map

The table deliberately separates a working model from a finished professional
tool. A placeholder or a few labels do not count as a complete trading window.

| Tool family in the public guide | Umicom status | Current or intended Umicom scope |
|---|---|---|
| Watchlist and quote monitor | **Available** | Framework trading view model, simulated instruments, selection command and linked Instrument source behaviour. |
| Chart | **Foundation** | OHLC evidence, chart panes, scales, annotations and drawings are modelled. Native studies, templates, crosshair, zoom and chart trading need further product work. See the public [real-time chart guide](https://www.ibkrguides.com/traderworkstation/create-a-real-time-chart.htm). |
| Market Depth / Level II | **Available** | Simulated bid and ask depth with a Framework trading view. Provider entitlements and real exchange books remain outside simulation. |
| Order Entry | **Available in simulation** | Guarded Framework order ticket and controller. Visibility never arms live routing. |
| Orders, executions and trade history | **Foundation** | Orders/blotter and execution projections exist. Filtering, saved columns, audit export and complete lifecycle drill-down need native surfaces. |
| Portfolio, positions and risk | **Foundation** | Positions, realised P&L and reusable portfolio/risk projection exist. Scenario analysis and provider reconciliation remain future work. |
| Scanner | **Foundation** | A canonical market-scanner view exists over current instruments. Saved scans, schedules, fundamentals and analyst criteria remain planned. |
| News | **Foundation** | Provider readiness and a safe empty state exist. No provider is silently assumed or bundled. |
| Alerts | **Planned** | Reuse Framework notification, policy and scheduling contracts for price, volume, risk and order-state alerts. |
| Time and Sales | **Planned** | Add a high-volume trade tape model with sequence checks, filters, pause/resume and bounded memory. |
| Option Chain and Strategy Builder | **Planned** | Model expiries, strikes, calls/puts, Greeks, multi-leg validation and strategy templates before writing the GTK panel. See the public [Option Chain guide](https://www.ibkrguides.com/traderworkstation/option-chain.htm). |
| Price ladder / Book Trader | **Planned** | Build on canonical depth and guarded order actions. Single-click actions must remain configurable, permissioned and simulation-first. See the public [BookTrader guide](https://www.ibkrguides.com/traderworkstation/open-booktrader.htm). |
| Rebalance Portfolio | **Planned** | Separate target generation, proposed orders, review evidence and explicit submission. Never treat an automatically generated order as approved. |
| Research and fundamentals | **Planned** | Use provider-neutral data contracts with licensing, provenance, freshness and unavailable states. |
| Account and connection status | **Foundation** | Framework health and account views exist. The application bar needs concise market-data, broker and simulation/live indicators. |

## What all Umicom applications inherit

These behaviours are not trading-specific and belong in Framework:

- named layouts and task-oriented templates;
- searchable New Window catalogues;
- edit, validate, save, cancel and lock lifecycles;
- show, hide, move, resize, float, close and restore operations;
- context links with source and destination roles;
- layout history, crash recovery and missing-monitor recovery;
- appearance, density, accessibility and keyboard movement;
- honest placeholders when an application has not supplied a renderer.

Studio applies File, Project, Symbol and Diagnostic contexts. TMS applies Deal,
Book and Position contexts. Bank applies Customer and Account contexts. Media
applications apply Document, Track and Timeline contexts. This is how one
Framework feature becomes a reusable Lego piece rather than copied product
logic.

## Delivery order

1. Persist lightweight customisation changes through the existing Framework
   semantic layout service.
2. Add reusable panel chrome: title, settings, context link, placement, detach,
   maximise and close.
3. Add keyboard-accessible docking targets and visual snap previews.
4. Complete monitor-aware floating windows and missing-monitor recovery.
5. Finish Trader tool renderers in risk order: audit/history, alerts, time and
   sales, options, ladder, rebalance, then provider-backed research.
6. Add professional settings for columns, number/date formats, density and
   accessibility without hiding financial precision by default.
7. Verify every live-capable action still passes permissions, validation,
   acknowledgement and audit recording independently of the layout system.

This order makes the reusable desktop foundation stable before adding more
high-risk trading interactions.
