#include <ast.h>
#include <errno.h>

/* ======================================== */

// ========== BIN tree

struct bin_tree_t {
    sln_token_t token;
    bin_tree_t* left;
    bin_tree_t* right;
};

void print_bin_tree(bin_tree_t* root, int depth=1) {
    if (!root)
        return;

    if (root->token.type == SLN_TOKEN_EOF)
        return;

    for (int i = 0; i < depth; ++i)
        printf(" ");
    print_token(root->token);

    if (root->left)
        print_bin_tree(root->left, depth+1);
    if (root->right)
        print_bin_tree(root->right, depth+1);
}



// ========== AST generator


bin_tree_t* generate_plus_minus_bin_tree(sln_token_buffer_t buffer) {
    if (buffer.num_of_tokens == 0UL) {
        perror("[generate_plus_minus_bin_tree]: Null pointer in args.");
        return (bin_tree_t*)NULL;
    }
        

    if (buffer.num_of_tokens % 2 != 0) {
        perror("[generate_plus_minus_bin_tree]: Invalid number of args.");
        return (bin_tree_t*)NULL;
    }
        

    bin_tree_t* root = new bin_tree_t;
    root->right = new bin_tree_t;
    root->right->token = buffer.tokens[0];
    for (size_t i = 1; i < buffer.num_of_tokens; i += 2) {
        if (buffer.tokens[i] != )
        root->token = buffer.tokens[i];

        bin_tree_t* left = new bin_tree_t;
        left->token = buffer.tokens[i];
    }

}



/* ======================================== */

const char* text = "1233 + 45645";

int main (void) {
    sln_token_buffer_t buffer = {0};

    errno = sln_tokenize(text, &buffer);
    if (errno != 0) {
        perror("Tokenization failed!\n");
    } else {
        printf("Tokenization successfull!\n");
    }

    bin_tree_t* root = new bin_tree_t;
    
    


    return 0;
}