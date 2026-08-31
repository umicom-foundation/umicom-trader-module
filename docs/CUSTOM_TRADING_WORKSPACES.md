<!--
Umicom Trader
File: docs/CUSTOM_TRADING_WORKSPACES.md

PURPOSE:
Explain how Trader uses Framework-owned editable layouts, the New Window
catalogue and linked context groups.

AUTHOR AND ORGANISATION:
Sammy Hegab
Umicom Foundation

LICENCE:
MIT
-->

# Custom Trading Workspaces

Umicom Trader now consumes the Framework-owned custom workspace controls. The
goal is a familiar professional workflow: choose a layout, unlock it, add the
trading windows needed for the task, arrange them, and lock the layout when
finished. The implementation and terminology are owned by Umicom Framework.

## What a user can do

1. Choose a predefined Trader layout from the layout selector.
2. Select **Edit Layout**.
3. Open **New Window** and search the Framework catalogue.
4. Choose Centre, Left, Right, Bottom or Top. A panel may also be marked as
   floating.
5. Add another Chart, Watchlist, Order Entry or other multi-instance tool. If a
   tool permits only one instance, choosing it again moves the existing panel
   to the selected region instead of creating a duplicate.
6. Select **Apply and Lock**, or select **Cancel** to restore the previous locked
   arrangement.

The New Window catalogue also contains Framework capabilities shared with other
applications. If Trader has no specialised renderer for one of them yet, the
workspace shows an honest placeholder. The catalogue contract exists; the
missing product renderer remains visible development work.

## Safety rules

Layout customisation does not change trading safety. Trader still starts in the
simulation environment, does not arm live routing by itself, and routes order
actions through the existing guarded trading controller. A window being visible
never grants an account permission or live-trading capability.

## Context-linked panels

The intended default trading link is an Instrument group:

- Watchlist and Scanner can be sources.
- Chart, Quote Details, News and Order Entry can be destinations.
- A panel can be bidirectional only when its behaviour truly supports it.

The reusable panel header now carries an independent linked-context identity.
The underlying Framework supports coloured source, destination and
bidirectional link roles; the visual colour chooser remains a separate native
interaction so it is not confused with panel placement.

## Product roadmap

- Persist user layouts and restore them at login.
- Add a complete categorized Trader New Window catalogue: watchlists, charts,
  market depth, time and sales, scanners, options tools, order management,
  portfolio/risk, research, news and education.
- Add panel-header placement, detach, close and colour-link controls.
- Add multi-monitor window recovery and “bring all windows to front”.
- Add named window groups and group-level minimize/restore.
- Add account, instrument and order context histories.
- Add layout import/export without including credentials or account secrets.
