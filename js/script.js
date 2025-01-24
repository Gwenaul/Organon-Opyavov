// script.js

const grid = document.getElementById('grid');
const colorPicker = document.getElementById('colorPicker');
const newGridButton = document.getElementById('newGridButton');
const animateButton = document.getElementById('animateButton');
const stopAnimationButton = document.getElementById('stopAnimationButton');
const prevButton = document.getElementById('prevButton');
const nextButton = document.getElementById('nextButton');

let allGrids = [];  // Pour stocker toutes les grilles
let currentGrid = [];  // La grille actuellement coloriée
let animationInterval;  // Variable pour l'intervalle d'animation
let isAnimating = false;  // Pour suivre l'état de l'animation
let currentGridIndex = 0;  // Index de la grille affichée
const rows = 8;
const cols = 20;
let isMouseDown = false;  // Variable pour savoir si la souris est enfoncée

// Fonction pour créer la grille interactive
function createGrid(rows, cols) {
    grid.innerHTML = ''; // Réinitialiser la grille

    for (let i = 0; i < rows * cols; i++) {
        const gridItem = document.createElement('div');
        gridItem.classList.add('grid-item');
        gridItem.style.backgroundColor = currentGrid[i] || '#fff'; // Charge la grille actuelle

        // Event listener pour "mousedown" (clic enfoncé gauche ou droit)
        gridItem.addEventListener('mousedown', (e) => {
            e.preventDefault(); // Empêche le comportement par défaut (menu contextuel)
            isMouseDown = true;

            if (e.button === 0) {
                // Clic gauche
                gridItem.style.backgroundColor = colorPicker.value;
                currentGrid[i] = colorPicker.value;
            } else if (e.button === 2) {
                // Clic droit
                gridItem.style.backgroundColor = '#fff'; // Efface la couleur (remet blanc)
                currentGrid[i] = '#fff';
            }

            allGrids[currentGridIndex] = [...currentGrid]; // Mise à jour de la grille dans allGrids
        });

        // Event listener pour "mousemove" (quand la souris se déplace sur la case)
        gridItem.addEventListener('mousemove', (e) => {
            if (isMouseDown) {
                if (e.button === 0 || e.buttons === 1) {
                    // Clic gauche
                    gridItem.style.backgroundColor = colorPicker.value;
                    currentGrid[i] = colorPicker.value;
                } else if (e.button === 2 || e.buttons === 2) {
                    // Clic droit
                    gridItem.style.backgroundColor = '#fff'; // Efface la couleur (remet blanc)
                    currentGrid[i] = '#fff';
                }

                allGrids[currentGridIndex] = [...currentGrid]; // Mise à jour de la grille dans allGrids
            }
        });

        // Empêcher le menu contextuel sur le clic droit
        gridItem.addEventListener('contextmenu', (e) => {
            e.preventDefault();
        });

        // Event listener pour "mouseup" (relâchement du clic)
        gridItem.addEventListener('mouseup', () => {
            isMouseDown = false;
        });

        grid.appendChild(gridItem);
    }

    // Event listener pour stopper le coloriage quand la souris quitte la grille
    grid.addEventListener('mouseleave', () => {
        isMouseDown = false;
    });
}


// Sauvegarder la grille actuelle et créer une nouvelle grille vierge
newGridButton.addEventListener('click', () => {
    allGrids.push([...currentGrid]);  // Sauvegarder la grille actuelle
    currentGrid = Array(rows * cols).fill('#fff');  // Créer une nouvelle grille vide
    createGrid(rows, cols);  // Met à jour l'interface avec une nouvelle grille
});

// Fonction pour afficher une grille spécifique
function showGrid(index) {
    currentGrid = [...allGrids[index]];  // Charger la grille actuelle à modifier
    createGrid(rows, cols);  // Afficher la grille
}

// Fonction pour démarrer l'animation
function startAnimation() {
    if (allGrids.length > 0 && !isAnimating) {
        isAnimating = true;
        animationInterval = setInterval(() => {
            showGrid(currentGridIndex);
            currentGridIndex = (currentGridIndex + 1) % allGrids.length;
        }, 500);
    }
}

// Fonction pour stopper l'animation
function stopAnimation() {
    if (isAnimating) {
        clearInterval(animationInterval);
        isAnimating = false;
    }
}

// Gestionnaire d'événement pour le bouton d'animation
animateButton.addEventListener('click', startAnimation);

// Gestionnaire d'événement pour le bouton de stop
stopAnimationButton.addEventListener('click', stopAnimation);

// Gestionnaire pour le bouton "Précédente"
prevButton.addEventListener('click', () => {
    stopAnimation();  // Stopper l'animation si elle est en cours
    currentGridIndex = (currentGridIndex - 1 + allGrids.length) % allGrids.length;
    showGrid(currentGridIndex);  // Afficher la grille précédente
});

// Gestionnaire pour le bouton "Suivante"
nextButton.addEventListener('click', () => {
    stopAnimation();  // Stopper l'animation si elle est en cours
    currentGridIndex = (currentGridIndex + 1) % allGrids.length;
    showGrid(currentGridIndex);  // Afficher la grille suivante
});

// Crée la première grille vide au chargement
createGrid(rows, cols);

// Ajouter un listener sur le document pour gérer le relâchement de la souris hors de la grille
document.addEventListener('mouseup', () => {
    isMouseDown = false;
});
