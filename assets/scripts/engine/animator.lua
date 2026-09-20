local a = {}
function a.Create(name, sprite)
  local s = Animator.CreateAnimator(name, sprite)
  return s
end

function a.PlayAnimation(animator, name, loops)
  Animator.PlayAnimation(animator, name, loops)
end

return a
