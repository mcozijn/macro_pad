document.addEventListener("DOMContentLoaded", function () {
  let overlay = document.getElementById("overlay");
  if (!overlay) {
    overlay = document.createElement("div");
    overlay.id = "overlay";
    document.body.appendChild(overlay);
  }

  function showHighlight(target) {
    target.scrollIntoView({ behavior: "smooth", block: "center" });
    target.classList.add("highlight-target");
    overlay.classList.add("active");

    overlay.addEventListener(
      "click",
      function () {
        overlay.classList.remove("active");
        target.classList.remove("highlight-target");
      },
      { once: true }
    );
  }

  document.querySelectorAll('a[href^="#"]').forEach(function (anchor) {
    anchor.addEventListener("click", function (event) {
      const href = anchor.getAttribute("href");
      const targetId = href.substring(1);
      const targetElement = document.getElementById(targetId);

      if (targetElement) {
        event.preventDefault();
        showHighlight(targetElement);
      }
    });
  });
});
