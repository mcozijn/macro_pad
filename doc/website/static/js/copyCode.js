// static/js/copy-code.js

document.addEventListener('DOMContentLoaded', () => {
    const codeBlocks = document.querySelectorAll('pre code');

    codeBlocks.forEach((codeBlock) => {
        const copyBtn = document.createElement('button');
        copyBtn.className = 'copy-button';
        copyBtn.type = 'button';
        copyBtn.innerText = '📋';

        const pre = codeBlock.parentNode;
        pre.style.position = 'relative';
        pre.appendChild(copyBtn);

        copyBtn.addEventListener('click', () => {
            const codeText = codeBlock.innerText;
            navigator.clipboard.writeText(codeText).then(() => {
                copyBtn.innerText = 'Copied!';
                setTimeout(() => {
                    copyBtn.innerText = '📋';
                }, 2000);
            }).catch(err => {
                console.error('Failed to copy: ', err);
            });
        });
    });
});
