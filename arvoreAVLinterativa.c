<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>AVL Tree Visualizer</title>
    <style>
        body {
            font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif;
            margin: 0;
            padding: 20px;
            background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
            min-height: 100vh;
            color: #333;
        }

        .container {
            max-width: 1200px;
            margin: 0 auto;
            background: rgba(255, 255, 255, 0.95);
            border-radius: 15px;
            padding: 30px;
            box-shadow: 0 20px 40px rgba(0, 0, 0, 0.1);
        }

        h1 {
            text-align: center;
            color: #2c3e50;
            margin-bottom: 30px;
            font-size: 2.5em;
            text-shadow: 2px 2px 4px rgba(0, 0, 0, 0.1);
        }

        .controls {
            display: flex;
            flex-wrap: wrap;
            gap: 15px;
            margin-bottom: 30px;
            justify-content: center;
            align-items: center;
        }

        .input-group {
            display: flex;
            align-items: center;
            gap: 10px;
        }

        input[type="number"] {
            padding: 12px;
            border: 2px solid #e0e0e0;
            border-radius: 8px;
            font-size: 16px;
            width: 80px;
            transition: border-color 0.3s;
        }

        input[type="number"]:focus {
            outline: none;
            border-color: #667eea;
        }

        button {
            padding: 12px 20px;
            border: none;
            border-radius: 8px;
            font-size: 16px;
            font-weight: 600;
            cursor: pointer;
            transition: all 0.3s ease;
            text-transform: uppercase;
            letter-spacing: 1px;
        }

        .btn-insert {
            background: linear-gradient(45deg, #4CAF50, #45a049);
            color: white;
        }

        .btn-remove {
            background: linear-gradient(45deg, #f44336, #da190b);
            color: white;
        }

        .btn-search {
            background: linear-gradient(45deg, #2196F3, #0b7dda);
            color: white;
        }

        .btn-clear {
            background: linear-gradient(45deg, #ff9800, #e68900);
            color: white;
        }

        button:hover {
            transform: translateY(-2px);
            box-shadow: 0 4px 8px rgba(0, 0, 0, 0.2);
        }

        .tree-container {
            min-height: 400px;
            background: #f8f9fa;
            border-radius: 10px;
            padding: 20px;
            margin: 20px 0;
            overflow-x: auto;
            border: 2px solid #e9ecef;
        }

        .node {
            position: absolute;
            background: linear-gradient(45deg, #667eea, #764ba2);
            color: white;
            border-radius: 50%;
            width: 50px;
            height: 50px;
            display: flex;
            align-items: center;
            justify-content: center;
            font-weight: bold;
            font-size: 14px;
            box-shadow: 0 4px 8px rgba(0, 0, 0, 0.2);
            transition: all 0.3s ease;
            border: 3px solid #fff;
        }

        .node:hover {
            transform: scale(1.1);
            box-shadow: 0 6px 12px rgba(0, 0, 0, 0.3);
        }

        .node-highlight {
            background: linear-gradient(45deg, #ff6b6b, #ee5a52) !important;
            animation: pulse 1s infinite;
        }

        @keyframes pulse {
            0%, 100% { transform: scale(1); }
            50% { transform: scale(1.05); }
        }

        .edge {
            position: absolute;
            background: #6c757d;
            height: 2px;
            transform-origin: left center;
            z-index: 1;
        }

        .info-panel {
            display: grid;
            grid-template-columns: repeat(auto-fit, minmax(250px, 1fr));
            gap: 20px;
            margin-top: 20px;
        }

        .info-card {
            background: linear-gradient(135deg, #667eea, #764ba2);
            color: white;
            padding: 20px;
            border-radius: 10px;
            box-shadow: 0 5px 15px rgba(0, 0, 0, 0.1);
        }

        .info-card h3 {
            margin-top: 0;
            font-size: 1.2em;
            border-bottom: 2px solid rgba(255, 255, 255, 0.3);
            padding-bottom: 10px;
        }

        .stats {
            display: flex;
            justify-content: space-between;
            margin: 10px 0;
        }

        .empty-tree {
            text-align: center;
            color: #6c757d;
            font-size: 1.2em;
            padding: 50px;
        }

        .range-controls {
            display: flex;
            gap: 10px;
            align-items: center;
            flex-wrap: wrap;
        }

        .range-results {
            background: #e8f4fd;
            border: 2px solid #bee5eb;
            border-radius: 8px;
            padding: 15px;
            margin-top: 15px;
        }

        .range-results h4 {
            margin: 0 0 10px 0;
            color: #0c5460;
        }

        .found-values {
            display: flex;
            flex-wrap: wrap;
            gap: 8px;
        }

        .value-tag {
            background: #17a2b8;
            color: white;
            padding: 5px 10px;
            border-radius: 15px;
            font-size: 14px;
            font-weight: 500;
        }
    </style>
</head>
<body>
    <div class="container">
        <h1>🌳 Interactive AVL Tree Visualizer</h1>
        
        <div class="controls">
            <div class="input-group">
                <input type="number" id="valueInput" placeholder="Enter value" min="1" max="999">
                <button class="btn-insert" onclick="insertValue()">Insert</button>
                <button class="btn-remove" onclick="removeValue()">Remove</button>
                <button class="btn-search" onclick="searchValue()">Search</button>
            </div>
            <button class="btn-clear" onclick="clearTree()">Clear Tree</button>
        </div>

        <div class="range-controls">
            <span>Range Search:</span>
            <input type="number" id="rangeLow" placeholder="Low" min="1" max="999" style="width: 70px;">
            <span>to</span>
            <input type="number" id="rangeHigh" placeholder="High" min="1" max="999" style="width: 70px;">
            <button class="btn-search" onclick="rangeSearch()">Search Range</button>
        </div>

        <div id="rangeResults" class="range-results" style="display: none;">
            <h4>Range Search Results:</h4>
            <div id="foundValues" class="found-values"></div>
        </div>

        <div class="tree-container" id="treeContainer">
            <div class="empty-tree">🌱 Tree is empty. Insert some values to get started!</div>
        </div>

        <div class="info-panel">
            <div class="info-card">
                <h3>📊 Tree Statistics</h3>
                <div class="stats">
                    <span>Nodes:</span>
                    <span id="nodeCount">0</span>
                </div>
                <div class="stats">
                    <span>Height:</span>
                    <span id="treeHeight">0</span>
                </div>
                <div class="stats">
                    <span>Root Balance:</span>
                    <span id="rootBalance">0</span>
                </div>
            </div>
            
            <div class="info-card">
                <h3>🔄 Last Operation</h3>
                <div id="lastOperation">No operations yet</div>
            </div>
            
            <div class="info-card">
                <h3>ℹ️ AVL Tree Info</h3>
                <p>AVL trees are self-balancing binary search trees where the heights of the two child subtrees of any node differ by at most one.</p>
            </div>
        </div>
    </div>

    <script>
        class AVLNode {
            constructor(key) {
                this.key = key;
                this.left = null;
                this.right = null;
                this.height = 1;
            }
        }

        class AVLTree {
            constructor() {
                this.root = null;
            }

            height(node) {
                return node ? node.height : 0;
            }

            balanceFactor(node) {
                return node ? this.height(node.left) - this.height(node.right) : 0;
            }

            rightRotate(y) {
                const x = y.left;
                const T2 = x.right;
                
                x.right = y;
                y.left = T2;
                
                y.height = Math.max(this.height(y.left), this.height(y.right)) + 1;
                x.height = Math.max(this.height(x.left), this.height(x.right)) + 1;
                
                return x;
            }

            leftRotate(x) {
                const y = x.right;
                const T2 = y.left;
                
                y.left = x;
                x.right = T2;
                
                x.height = Math.max(this.height(x.left), this.height(x.right)) + 1;
                y.height = Math.max(this.height(y.left), this.height(y.right)) + 1;
                
                return y;
            }

            insert(node, key) {
                if (!node) return new AVLNode(key);

                if (key < node.key) {
                    node.left = this.insert(node.left, key);
                } else if (key > node.key) {
                    node.right = this.insert(node.right, key);
                } else {
                    return node; // Duplicate keys not allowed
                }

                node.height = 1 + Math.max(this.height(node.left), this.height(node.right));
                const balance = this.balanceFactor(node);

                // Left Left Case
                if (balance > 1 && key < node.left.key) {
                    return this.rightRotate(node);
                }

                // Right Right Case
                if (balance < -1 && key > node.right.key) {
                    return this.leftRotate(node);
                }

                // Left Right Case
                if (balance > 1 && key > node.left.key) {
                    node.left = this.leftRotate(node.left);
                    return this.rightRotate(node);
                }

                // Right Left Case
                if (balance < -1 && key < node.right.key) {
                    node.right = this.rightRotate(node.right);
                    return this.leftRotate(node);
                }

                return node;
            }

            minValueNode(node) {
                let current = node;
                while (current.left) {
                    current = current.left;
                }
                return current;
            }

            remove(node, key) {
                if (!node) return node;

                if (key < node.key) {
                    node.left = this.remove(node.left, key);
                } else if (key > node.key) {
                    node.right = this.remove(node.right, key);
                } else {
                    if (!node.left || !node.right) {
                        const temp = node.left || node.right;
                        if (!temp) {
                            node = null;
                        } else {
                            node = temp;
                        }
                    } else {
                        const temp = this.minValueNode(node.right);
                        node.key = temp.key;
                        node.right = this.remove(node.right, temp.key);
                    }
                }

                if (!node) return node;

                node.height = 1 + Math.max(this.height(node.left), this.height(node.right));
                const balance = this.balanceFactor(node);

                // Left Left Case
                if (balance > 1 && this.balanceFactor(node.left) >= 0) {
                    return this.rightRotate(node);
                }

                // Left Right Case
                if (balance > 1 && this.balanceFactor(node.left) < 0) {
                    node.left = this.leftRotate(node.left);
                    return this.rightRotate(node);
                }

                // Right Right Case
                if (balance < -1 && this.balanceFactor(node.right) <= 0) {
                    return this.leftRotate(node);
                }

                // Right Left Case
                if (balance < -1 && this.balanceFactor(node.right) > 0) {
                    node.right = this.rightRotate(node.right);
                    return this.leftRotate(node);
                }

                return node;
            }

            search(node, key) {
                if (!node || node.key === key) {
                    return node;
                }

                if (key < node.key) {
                    return this.search(node.left, key);
                }

                return this.search(node.right, key);
            }

            rangeSearch(node, low, high, result = []) {
                if (!node) return result;

                if (node.key >= low && node.key <= high) {
                    result.push(node.key);
                }

                if (node.key > low) {
                    this.rangeSearch(node.left, low, high, result);
                }

                if (node.key < high) {
                    this.rangeSearch(node.right, low, high, result);
                }

                return result.sort((a, b) => a - b);
            }

            countNodes(node) {
                if (!node) return 0;
                return 1 + this.countNodes(node.left) + this.countNodes(node.right);
            }
        }

        const tree = new AVLTree();
        let highlightedNode = null;

        function insertValue() {
            const input = document.getElementById('valueInput');
            const value = parseInt(input.value);
            
            if (isNaN(value) || value < 1) {
                alert('Please enter a valid positive number');
                return;
            }

            if (tree.search(tree.root, value)) {
                updateLastOperation(`Value ${value} already exists in the tree`);
                highlightNode(value);
                return;
            }

            tree.root = tree.insert(tree.root, value);
            input.value = '';
            updateLastOperation(`Inserted ${value}`);
            visualizeTree();
            updateStats();
            highlightNode(value);
        }

        function removeValue() {
            const input = document.getElementById('valueInput');
            const value = parseInt(input.value);
            
            if (isNaN(value) || value < 1) {
                alert('Please enter a valid positive number');
                return;
            }

            if (!tree.search(tree.root, value)) {
                updateLastOperation(`Value ${value} not found in tree`);
                return;
            }

            tree.root = tree.remove(tree.root, value);
            input.value = '';
            updateLastOperation(`Removed ${value}`);
            visualizeTree();
            updateStats();
        }

        function searchValue() {
            const input = document.getElementById('valueInput');
            const value = parseInt(input.value);
            
            if (isNaN(value) || value < 1) {
                alert('Please enter a valid positive number');
                return;
            }

            const found = tree.search(tree.root, value);
            updateLastOperation(`Search for ${value}: ${found ? 'Found' : 'Not found'}`);
            
            if (found) {
                highlightNode(value);
            }
        }

        function clearTree() {
            tree.root = null;
            updateLastOperation('Tree cleared');
            visualizeTree();
            updateStats();
        }

        function rangeSearch() {
            const low = parseInt(document.getElementById('rangeLow').value);
            const high = parseInt(document.getElementById('rangeHigh').value);
            
            if (isNaN(low) || isNaN(high) || low > high) {
                alert('Please enter valid range values (low ≤ high)');
                return;
            }

            const results = tree.rangeSearch(tree.root, low, high);
            const resultsDiv = document.getElementById('rangeResults');
            const valuesDiv = document.getElementById('foundValues');
            
            if (results.length === 0) {
                valuesDiv.innerHTML = '<span style="color: #6c757d; font-style: italic;">No values found in range</span>';
            } else {
                valuesDiv.innerHTML = results.map(val => `<span class="value-tag">${val}</span>`).join('');
            }
            
            resultsDiv.style.display = 'block';
            updateLastOperation(`Range search [${low}, ${high}]: Found ${results.length} values`);
        }

        function highlightNode(value) {
            // Remove previous highlight
            if (highlightedNode) {
                highlightedNode.classList.remove('node-highlight');
            }
            
            // Find and highlight new node
            const nodeElement = document.querySelector(`[data-value="${value}"]`);
            if (nodeElement) {
                nodeElement.classList.add('node-highlight');
                highlightedNode = nodeElement;
                
                // Remove highlight after 2 seconds
                setTimeout(() => {
                    if (highlightedNode) {
                        highlightedNode.classList.remove('node-highlight');
                        highlightedNode = null;
                    }
                }, 2000);
            }
        }

        function visualizeTree() {
            const container = document.getElementById('treeContainer');
            container.innerHTML = '';

            if (!tree.root) {
                container.innerHTML = '<div class="empty-tree">🌱 Tree is empty. Insert some values to get started!</div>';
                return;
            }

            const positions = calculatePositions(tree.root);
            drawTree(container, tree.root, positions);
        }

        function calculatePositions(root) {
            const positions = new Map();
            
            function inorder(node, depth = 0) {
                if (!node) return [];
                
                const leftNodes = inorder(node.left, depth + 1);
                const rightNodes = inorder(node.right, depth + 1);
                
                return [...leftNodes, { node, depth }, ...rightNodes];
            }
            
            const nodes = inorder(root);
            const levelWidth = 100;
            const nodeWidth = 60;
            
            nodes.forEach((item, index) => {
                positions.set(item.node, {
                    x: 50 + index * levelWidth,
                    y: 50 + item.depth * 80
                });
            });
            
            return positions;
        }

        function drawTree(container, node, positions) {
            if (!node) return;
            
            const pos = positions.get(node);
            
            // Create node element
            const nodeEl = document.createElement('div');
            nodeEl.className = 'node';
            nodeEl.style.left = pos.x + 'px';
            nodeEl.style.top = pos.y + 'px';
            nodeEl.textContent = node.key;
            nodeEl.setAttribute('data-value', node.key);
            nodeEl.title = `Value: ${node.key}\nHeight: ${node.height}\nBalance: ${tree.balanceFactor(node)}`;
            container.appendChild(nodeEl);
            
            // Draw edges to children
            if (node.left) {
                const childPos = positions.get(node.left);
                drawEdge(container, pos.x + 25, pos.y + 25, childPos.x + 25, childPos.y + 25);
                drawTree(container, node.left, positions);
            }
            
            if (node.right) {
                const childPos = positions.get(node.right);
                drawEdge(container, pos.x + 25, pos.y + 25, childPos.x + 25, childPos.y + 25);
                drawTree(container, node.right, positions);
            }
        }

        function drawEdge(container, x1, y1, x2, y2) {
            const edge = document.createElement('div');
            edge.className = 'edge';
            
            const length = Math.sqrt((x2 - x1) ** 2 + (y2 - y1) ** 2);
            const angle = Math.atan2(y2 - y1, x2 - x1) * 180 / Math.PI;
            
            edge.style.left = x1 + 'px';
            edge.style.top = y1 + 'px';
            edge.style.width = length + 'px';
            edge.style.transform = `rotate(${angle}deg)`;
            
            container.appendChild(edge);
        }

        function updateStats() {
            document.getElementById('nodeCount').textContent = tree.countNodes(tree.root);
            document.getElementById('treeHeight').textContent = tree.height(tree.root);
            document.getElementById('rootBalance').textContent = tree.root ? tree.balanceFactor(tree.root) : 0;
        }

        function updateLastOperation(message) {
            document.getElementById('lastOperation').textContent = message;
        }

        // Event listeners
        document.getElementById('valueInput').addEventListener('keypress', function(e) {
            if (e.key === 'Enter') {
                insertValue();
            }
        });

        document.getElementById('rangeLow').addEventListener('keypress', function(e) {
            if (e.key === 'Enter') {
                rangeSearch();
            }
        });

        document.getElementById('rangeHigh').addEventListener('keypress', function(e) {
            if (e.key === 'Enter') {
                rangeSearch();
            }
        });

        // Initialize
        updateStats();
    </script>
</body>
</html>
