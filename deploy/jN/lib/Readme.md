# Feature Architecture




```plantuml
node GitHub{
    component Registry as reg <<repository>>{
        file plugins.json
    }
}

node Npp{
    component ActivationCtx as ctx
    component PluginManager as pm
    component PluginUpdater as pu
    component Start as start

    start -> pm
    start -l-> pu
    pm -d-> ctx

    frame plugins{
        frame MyPlugin{
            file package.json as p
            file main.js as m {
                rectangle "activate(ctx)" as act
            }
            m - p
            act -u-> ctx
        }
    }

    pu -d-> plugins
    pm -d-> plugins
    pu -u-> reg
}
```

### Plugin Manager

Plugin Manager takes care about the installed and running plugins. It activates plugins at the start and activates fresh downloaded plugins.

### Plugin Updater

Plugin Updater takes care about downloading and updating of existing
plugins.

