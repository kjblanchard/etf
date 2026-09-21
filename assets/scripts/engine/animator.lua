local c = require("constants")
local a = {}

local animationData = {}

--TODO probably clean this up for better error handling
function a.Create(name, sprite)
  local s = Animator.CreateAnimator(name, animationData[name], sprite)
  return s
end

function a.CreateAnimatorData(name)
  return Animator.CreateAnimationData(name)
end

function a.PreloadAnimatorData()
  for _, name in ipairs(c.PreloadAnimators) do
    animationData[name] = a.CreateAnimatorData(name)
  end
end

function a.PlayAnimation(animator, name, loops)
  if not animator then return end
  Animator.PlayAnimation(animator, name, loops)
end

return a
