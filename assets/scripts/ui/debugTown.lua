return {
  DebugTownPanel = {
    children = {
      testniner = {
        t = "nineslice",
        file = "uibase",
        color = { 60, 0, 100, 245 },
        rect = { 0, 0, 30, 30 },
        children = {
          thing1 = {
            t = "image",
            file = "player1",
            priority = 1,
            color = { 255, 255, 255, 235 },
            rect = { 5, 5, 8, 8 },
            srcRect = { 0, 0, 26, 36 }
          },
          thing3 = {
            t = "text",
            text = "Hello world!",
            size = 16,
            priority = 0,
            color = { 255, 255, 255, 255 },
            rect = { 0, 0, 144, 144 },
            centered = true
          }
        },
      },
      thing2 = {
        t = "anim",
        file = "player1",
        priority = 0,
        color = { 80, 0, 120, 235 },
        rect = { 0, 0, 16, 16 },
        srcRect = { 0, 0, 26, 36 }
      }
    }
  },



}
