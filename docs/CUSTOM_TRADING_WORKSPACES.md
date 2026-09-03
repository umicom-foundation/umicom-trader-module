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
5. Add another Chart, Watchlist, Scanner or Time and Sales window. Guarded
   Order Entry, risk and account controls remain singletons. Choosing a
   singleton again moves the existing panel instead of creating a duplicate.
6. Select **Apply and Lock**, or select **Cancel** to restore the previous locked
   arrangement.
7. Select **Save** to create a recovery checkpoint for the current session.
   Select **Restore** to return to that committed checkpoint.

Each panel header also provides Move, Context and Settings actions. These open
one shared editor where the user can select a region, choose a linked context,
enable auto-hide where supported, and apply the complete change atomically. If
one part is not allowed, no partial arrangement is kept.

The New Window catalogue also contains Framework capabilities shared with other
applications. Scanner, predictive research, news, context inspection, strategy,
replay, time-and-sales readiness, economic-event readiness, fundamentals and
research output now have reusable Framework view models. A view which needs an
external provider shows an honest unavailable state instead of invented rows.
Options analysis remains visible unfinished work and may still render a clear
placeholder.

Trader also exposes portable layout text through thin wrapper functions. The
Framework validates tools, linked groups and conflict policy, then replaces a
known canonical layout atomically. Trader does not parse or repair layout text.

## Safety rules

Layout customisation does not change trading safety. Trader still starts in the
simulation environment, does not arm live routing by itself, and routes order
actions through the existing guarded trading controller. A window being visible
never grants an account permission or live-trading capability.

The standard Trader window now receives its starting dimensions from the
Framework presentation catalogue. Framework fits the preferred size into a
laptop-friendly area and marks the window as compact when optional panels
should collapse. Trading surfaces also declare whether they are offline,
network-assisted or streaming and whether they handle public, internal,
confidential or restricted data. A live restricted surface must pass both
checks before activation; visual presence alone never bypasses the trading
controller.

## Context-linked panels

The intended default trading link is an Instrument group:

- Watchlist and Scanner can be sources.
- Chart, Quote Details, News and Order Entry can be destinations.
- A panel can be bidirectional only when its behaviour truly supports it.

The reusable panel header carries an independent linked-context identity. The
Framework derives available groups from Trader's experience definition and
supports source, destination and bidirectional link roles. Placement and linked
meaning remain separate fields even though one editor presents both choices.

## Product roadmap

- Connect the Framework checkpoint text to durable user settings and restore it
  at login.
- Attach accepted public-trade, economic-event and fundamental-data providers
  to the existing honest capability views.
- Complete the options analysis model and dense native table renderer.
- Render floating panels as native detached windows and recover monitor changes.
- Add multi-monitor window recovery and “bring all windows to front”.
- Add named window groups and group-level minimize/restore.
- Add account, instrument and order context histories.
- Add layout import/export without including credentials or account secrets.
