const P_SIZE = 4;
const PIVOT_SIZE = 10;
const WIDTH = 800;
const HEIGHT = 600;
const QTD = 1000;

document.addEventListener('DOMContentLoaded', function() {
  const canvas = document.getElementById('canvas');
  const ctx = canvas.getContext('2d');

  const p1 = { x: 10, y: 300 };
  const p2 = { x: 250, y: 10 };
  const p3 = { x: 450, y: 10 };
  const p4 = { x: 600, y: 300 };

  const points = [p1, p2, p3, p4];
  let dragging = null;

  canvas.addEventListener('mousedown', (e) => {
    for (let point of points) {
      if (e.clientX >= point.x && e.clientX <= point.x + PIVOT_SIZE
        && e.clientY >= point.y && e.clientY <= point.y + PIVOT_SIZE) {
        dragging = point;
      }
    }
  });

  canvas.addEventListener('mousemove', (e) => {
    if (dragging) {
      dragging.x = e.clientX;
      dragging.y = e.clientY;
    }
  });

  canvas.addEventListener('mouseup', () => {
    dragging = null;
  });

  setInterval(loop, 60 / 1000);

  function loop() {
    clear();

    for (let t = 0; t < QTD; t++) {
      const p1_2 = lerpVec2(p1, p2, sin(t));
      const p2_3 = lerpVec2(p2, p3, sin(t));
      const p3_4 = lerpVec2(p3, p4, sin(t));
      const pA = lerpVec2(p1_2, p2_3, sin(t));
      const pB = lerpVec2(p2_3, p3_4, sin(t));
      const pC = lerpVec2(pA, pB, sin(t));
      drawPivot(p1);
      drawPivot(p2);
      drawPivot(p3);
      drawPivot(p4);
      const r = (pC.x / WIDTH) * 255;
      const g = (pC.y / HEIGHT) * 255;
      const b = (pC.y / HEIGHT) * 255;
      drawRect(pC, `rgb(${r}, ${g}, ${b})`);
    }

    drawLine(p1, p2);
    drawLine(p3, p4);
  }

  function sin(t) {
    return (Math.sin(t) + 1) * .5;
  }

  function scaleVec2(p, s) {
    return { x: p.x * s, y: p.y * s };
  }

  function subVec2(p1, p2) {
    return { x: p1.x - p2.x, y: p1.y - p2.y };
  }

  function addVec2(p1, p2) {
    return { x: p1.x + p2.x, y: p1.y + p2.y };
  }

  function lerpVec2(p1, p2, f) {
    return addVec2(p1, scaleVec2(subVec2(p2, p1), f));
  }

  function clear() {
    ctx.beginPath();
    ctx.fillStyle = 'rgb(255, 255, 255)';
    ctx.fillRect(0, 0, WIDTH, HEIGHT);
    ctx.closePath();
  }

  function drawLine(p1, p2) {
    ctx.beginPath();
    ctx.strokeStyle = 'rgb(18, 18, 18)';
    ctx.lineWidth = 1;
    ctx.moveTo(p1.x + PIVOT_SIZE * .5, p1.y + PIVOT_SIZE * .5);
    ctx.lineTo(p2.x + PIVOT_SIZE * .5, p2.y + PIVOT_SIZE * .5);
    ctx.stroke();
    ctx.closePath();
  }

  function drawPivot(p) {
    drawRect(p, undefined, PIVOT_SIZE, PIVOT_SIZE);
  }

  function drawRect(p, color, w, h) {
    ctx.beginPath();
    ctx.fillStyle = color ?? 'rgb(18, 18, 18)';
    ctx.fillRect(p.x, p.y, w ?? P_SIZE, h ?? P_SIZE);
    ctx.closePath();
  }
});
