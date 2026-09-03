<!--
Umicom Trader
File: docs/TRADING_WORKSTATION_CAPABILITY_MAP.md

PURPOSE:
Explain the current and planned Umicom Trader workstation capabilities without
claiming unfinished application surfaces are complete.

AUTHOR AND ORGANISATION:
Sammy Hegab
Umicom Foundation

LICENCE:
MIT
-->

# Umicom Trader Workstation Capability Map

## Why this document exists

A professional trading application contains many windows, but each user needs
a different arrangement. Umicom Trader therefore combines task-oriented
layouts, a searchable window catalogue, safe layout editing and linked market
context. These are original Umicom features built from reusable Framework
contracts.

The status words are intentionally strict:

- **Available** means a usable Framework or Trader implementation exists.
- **Foundation** means the reusable contract exists but some native interaction
  or external-provider work remains.
- **Planned** means the behaviour is designed but must not be presented as
  complete.

## Workspace and window behaviour

| Capability | Status | Umicom implementation |
|---|---|---|
| Searchable New Window catalogue | **Available** | `UmiUiWindowCatalogue` supports categories, recent use, singleton detection and bounded results. |
| Guarded layout edit mode | **Available** | `UmiUiWorkspaceCustomisation` owns begin, apply, cancel and lock. |
| Named task layouts | **Foundation** | Framework supports add, clone, rename, activate and remove; final creation dialogs remain product work. |
| Docked and floating panels | **Foundation** | Left, centre, right, top, bottom and floating placement are explicit. Native detached monitor windows remain planned. |
| Tab stacks | **Available** | `stack_id` identifies visual tab grouping independently of placement. |
| Linked market context | **Available** | `context_group_id` and `UmiUiWindowGroupStore` share typed subjects without changing panel placement. |
| Pin, maximise, float and close actions | **Available** | Shared panel chrome publishes toolkit-neutral actions and the GTK host routes supported actions into the active layout. |
| Save and restore compatibility | **Available** | Layout schema 3 stores placement, stack, linked context, pinned and resizable state while reading schema 2 files. Import is dependency-checked and atomic. The GTK workstation has a session checkpoint; durable user-settings storage remains the next integration. |
| Multi-monitor recovery | **Foundation** | The semantic layout service owns monitor topology and recovery; native detached-window integration remains. |
| Appearance and density | **Foundation** | Reusable theme and density contracts exist; complete table-column profiles remain. |

## Trading window families

| Window family | Status | Current or intended scope |
|---|---|---|
| Watchlist and quotes | **Available** | Simulated instruments, selection and linked Instrument source behaviour. |
| Chart | **Available** | The workspace retains a bounded chronological candle history per instrument. The native resizable chart renders real candles and lets the user switch between candle-only, simple-average and exponential-average views with a configurable period. Advanced drawing interaction remains a further increment. |
| Market depth | **Available** | Simulated bid and ask depth with provider-neutral view models. |
| Order entry | **Available in simulation** | Guarded order ticket and controller; visibility never enables live routing. |
| Orders and executions | **Foundation** | Order, execution and history projections exist; complete filtering and drill-down remain. |
| Portfolio and risk | **Foundation** | Positions, profit and loss, portfolio and risk projections exist; reconciliation remains. |
| Scanner | **Available** | The canonical scanner view renders in GTK with filters, coverage and data-quality state; saved and scheduled scans remain an extension. |
| News and research | **Foundation** | Provider-aware empty states exist; no external source is silently assumed. |
| Alerts | **Available** | Framework owns bounded price rules, directional crossing detection, quote-driven activation, pause, resume, acknowledgement, removal and occurrence history. The native panel exposes the complete lifecycle for the selected instrument and never submits orders. |
| Time and sales | **Foundation** | A canonical movable Framework view now reports linked context and provider readiness without confusing account executions with public trades; add a bounded, sequence-checked feed, filters and pause/resume. |
| Economic calendar | **Foundation** | A reusable provider-neutral view reports linked context and an honest unavailable state until an accepted event adapter is configured. |
| Fundamentals | **Foundation** | The view shows canonical instrument and market identity now; company accounts and classification data wait behind an accepted provider adapter. |
| Options analysis | **Planned** | Model expiries, strikes, calls, puts, sensitivities and reviewed multi-leg orders. |
| Price ladder | **Foundation** | A critical movable panel contract now exists; build its gestures on market depth and guarded order actions with configurable permissions. |
| Portfolio rebalance | **Planned** | Separate targets, proposals, review evidence and explicit submission. |

## The three identities on every panel

Every new panel developer must understand this separation:

1. `placement_id` says where the panel is docked.
2. `stack_id` says which visual tab group contains it.
3. `context_group_id` says which selected instrument, account, order or other
   subject it follows.

Changing one identity must not silently change either of the others. This rule
is shared by Studio, TMS, Bank, media applications and future Umicom products.

## Safety rules

- Trader starts in simulation unless an independently governed environment is
  selected and accepted.
- Showing an order panel never grants an account permission.
- Layout actions do not bypass order validation, risk, acknowledgement or
  audit recording.
- External data and execution adapters remain separate from visual panels.
- An unavailable provider is shown honestly as unavailable.

## Recommended delivery order

1. Connect portable checkpoint text to durable settings and crash recovery.
2. Add keyboard docking, visible drop targets and screen-reader announcements.
3. Finish detached windows and monitor recovery.
4. Extend time and sales, options, ladder and rebalance models behind the new
   canonical panel identities, then add indicator and drawing alert providers
   to the existing price-alert lifecycle.
5. Add provider adapters behind capability, safety and reconciliation gates.
6. Validate every live-capable action independently of the layout system.
