function updateTime() {
    var now = new Date();
    var hours = now.getHours().toString().padStart(2, '0');
    var minutes = now.getMinutes().toString().padStart(2, '0');
    var seconds = now.getSeconds().toString().padStart(2, '0');
    var timeString = hours + ":" + minutes + ":" + seconds;
    document.getElementById('current-time').innerHTML = timeString;
}

document.addEventListener('DOMContentLoaded', (event) => {
    updateTime();
    setInterval(updateTime, 1000); // Cập nhật mỗi giây
});
function updateLastUpdatedTime() {
    const now = new Date();
    const lastUpdatedTime = now.toLocaleString();
    document.getElementById('last-updated1').textContent = lastUpdatedTime;
    document.getElementById('last-updated2').textContent = lastUpdatedTime;
    document.getElementById('last-updated3').textContent = lastUpdatedTime;
}

// Gọi hàm này khi ảnh được hiển thị
async function displayImage(imagePath, imgElementId) {
    const imageRef = ref(storage, imagePath);
    try {
        const url = await getDownloadURL(imageRef);
        document.getElementById(imgElementId).src = url;
        updateLastUpdatedTime(); // Cập nhật thời gian khi ảnh được hiển thị
    } catch (error) {
        console.error('Error fetching image URL:', error);
    }
}
// Gọi hàm này khi trang được tải
window.addEventListener('load', () => {
    updateLastUpdatedTime(); // Cập nhật ngay khi trang được tải
});
document.addEventListener('DOMContentLoaded', (event) => {
    // Get all modals
    var modals = document.querySelectorAll('.modal');
    
    // Get all buttons that open the modals
    var btns = document.querySelectorAll('.detail-btn');
    
    // Get all <span> elements that close the modals
    var spans = document.querySelectorAll('.close');
    
    // When the user clicks on a button, open the modal
    btns.forEach((btn) => {
        btn.addEventListener('click', function() {
            var modalId = this.getAttribute('data-modal');
            document.getElementById(modalId).style.display = "block";
        });
    });
    
    // When the user clicks on <span> (x), close the modal
    spans.forEach((span) => {
        span.addEventListener('click', function() {
            this.parentElement.parentElement.style.display = "none";
        });
    });
    
    // When the user clicks anywhere outside of the modal, close it
    window.onclick = function(event) {
        if (event.target.classList.contains('modal')) {
            event.target.style.display = "none";
        }
    }
});
function updateContent() {
    if (!isModalOpen()) {
        location.reload();
    }
}

// Check if any modal is open
function isModalOpen() {
    return Array.from(document.querySelectorAll('.modal')).some(modal => modal.style.display === 'block');
}
function displayModalData(elementId, data) {
    const dataContainer = document.getElementById(elementId);
    if (dataContainer) {
        dataContainer.innerHTML = `
            <p>Nhiệt độ hiện tại: ${data.nhietdo}</p>
            <p>Độ ẩm hiện tại: ${data.doam}</p>
        `;
    }
}
// Call updateContent every 30 seconds
setInterval(updateContent, 30000);